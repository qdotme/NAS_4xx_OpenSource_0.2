<?php
// ensure this file is being included by a parent file
if( !defined( '_JEXEC' ) && !defined( '_VALID_MOS' ) ) die( 'Restricted access' );
/**
 * @version $Id: list.php,v 1.5 2007/09/12 06:51:06 wiley Exp $
 * @package eXtplorer
 * @copyright soeren 2007
 * @author The eXtplorer project (http://sourceforge.net/projects/extplorer)
 * @author The  The QuiX project (http://quixplorer.sourceforge.net)
 *
 * @license
 * The contents of this file are subject to the Mozilla Public License
 * Version 1.1 (the "License"); you may not use this file except in
 * compliance with the License. You may obtain a copy of the License at
 * http://www.mozilla.org/MPL/
 *
 * Software distributed under the License is distributed on an "AS IS"
 * basis, WITHOUT WARRANTY OF ANY KIND, either express or implied. See the
 * License for the specific language governing rights and limitations
 * under the License.
 *
 * Alternatively, the contents of this file may be used under the terms
 * of the GNU General Public License Version 2 or later (the "GPL"), in
 * which case the provisions of the GPL are applicable instead of
 * those above. If you wish to allow use of your version of this file only
 * under the terms of the GPL and not to allow others to use
 * your version of this file under the MPL, indicate your decision by
 * deleting  the provisions above and replace  them with the notice and
 * other provisions required by the GPL.  If you do not delete
 * the provisions above, a recipient may use your version of this file
 * under either the MPL or the GPL."
 *
 * Directory-Listing Functions
 */
//------------------------------------------------------------------------------
// HELPER FUNCTIONS (USED BY MAIN FUNCTION 'list_dir', SEE BOTTOM)
function make_list(&$_list1, &$_list2) {    // make list of files
  $list = array();

  if($GLOBALS["direction"]=="ASC") {
    $list1 = $_list1;
    $list2 = $_list2;
  } else {
    $list1 = $_list2;
    $list2 = $_list1;
  }

  if(is_array($list1)) {
    while (list($key, $val) = each($list1)) {
      $list[$key] = $val;
    }
  }

  if(is_array($list2)) {
    while (list($key, $val) = each($list2)) {
      $list[$key] = $val;
    }
  }

  return $list;
}

/**
 * make tables & place results in reference-variables passed to function
 * also 'return' total filesize & total number of items
 *
 * @param string $dir
 * @param array $dir_list
 * @param array $file_list
 * @param int $tot_file_size
 * @param int $num_items
 */
function get_dircontents($dir, &$dir_list, &$file_list, &$tot_file_size, &$num_items) {            // make table of files in dir
  $homedir = realpath($GLOBALS['home_dir']);
  $tot_file_size = $num_items = 0;
  $rel_shrname = get_rel_shrname($dir);
  $dir_list = $file_list = array();

  // check permission at specified path
  if($rel_shrname!="ext_root") {
    if (!in_allowed($rel_shrname)) return; // reject to get item(s)
  }

  // Open directory
  $handle = @$GLOBALS['ext_File']->opendir(get_abs_dir($dir));

  if($handle===false && $dir=="") {
    $handle = @$GLOBALS['ext_File']->opendir($homedir . $GLOBALS['separator']);
  }

  if($handle===false) {
    ext_Result::sendResult('list', false, $dir.": ".ext_Lang::err( 'opendir' ));
  }

  // Read directory
  while(($new_item = @$GLOBALS['ext_File']->readdir($handle))!==false) {
    if( is_array( $new_item ))  {
      $abs_new_item = $new_item;
    } else {
      $abs_new_item = get_abs_item($dir, $new_item);
    }

    if ($new_item == "." || $new_item == "..") continue;

    if(!get_show_item($dir, $new_item)) continue;

    if($rel_shrname!="ext_root") {
      $new_file_size = @$GLOBALS['ext_File']->filesize($abs_new_item);
      $tot_file_size += $new_file_size;
      $num_items++;
    }

    $new_item_name = $new_item;

    if(get_is_dir($abs_new_item)) {
      // to check relative share which be mounted in system now
      if ($rel_shrname=="ext_root") {
        if(!is_available(get_rel_shrname($abs_new_item))) continue;
      }

      if($GLOBALS["order"]=="modified") {
        $dir_list[$new_item_name] =
          @$GLOBALS['ext_File']->filemtime($abs_new_item);
      } else {  // order == "size", "type" or "name"
        $dir_list[$new_item_name] = $new_item;
      }
    } elseif($rel_shrname!="ext_root") {
      if($GLOBALS["order"]=="size") {
        $file_list[$new_item_name] = $new_file_size;
      } elseif($GLOBALS["order"]=="modified") {
        $file_list[$new_item_name] =
          @$GLOBALS['ext_File']->filemtime($abs_new_item);
      } elseif($GLOBALS["order"]=="type") {
        $file_list[$new_item_name] =
          get_mime_type( $abs_new_item, "type");
      } else {  // order == "name"
        $file_list[$new_item_name] = $new_item;
      }
    }
  }

  @$GLOBALS['ext_File']->closedir($handle);

  // sort
  if(is_array($dir_list)) {
    if($GLOBALS["order"]=="modified") {
      if($GLOBALS["direction"]=="ASC") arsort($dir_list);
      else asort($dir_list);
    } else {  // order == "size", "type" or "name"
      if($GLOBALS["direction"]=="ASC") ksort($dir_list);
      else krsort($dir_list);
    }
  }

  // sort
  if(is_array($file_list) && $rel_shrname!="ext_root") {
    if($GLOBALS["order"]=="modified") {
      if($GLOBALS["direction"]=="ASC") arsort($file_list);
      else asort($file_list);
    } elseif($GLOBALS["order"]=="size" || $GLOBALS["order"]=="type") {
      if($GLOBALS["direction"]=="ASC") asort($file_list);
      else arsort($file_list);
    } else {  // order == "name"
      if($GLOBALS["direction"]=="ASC") ksort($file_list);
      else krsort($file_list);
    }
  }
  if( $GLOBALS['start'] > $num_items ) {
    $GLOBALS['start'] = 0;
  }
}
/**
 * This function assembles an array (list) of files or directories in the directory specified by $dir
 * The result array is send using JSON
 *
 * @param string $dir
 * @param string $sendWhat Can be "files" or "dirs"
 */
function send_dircontents($dir, $sendWhat='files') {  // print table of files
  global $dir_up, $mainframe;

  // make file & dir tables, & get total filesize & number of items
  get_dircontents($dir, $dir_list, $file_list, $tot_file_size, $num_items);
  if( $sendWhat == 'files') {
    $list = $file_list;
  } elseif( $sendWhat == 'dirs') {
    $list = $dir_list;
  } else {
    $list = make_list( $dir_list, $file_list );
  }

  $i = 0;
  $toggle = false;
  $items['totalCount'] = count($list);
  $items['items'] = array();
  $dirlist = array();

  if( $sendWhat != 'dirs' ) {
    // Replaced array_splice, because it resets numeric indexes (like files or dirs with a numeric name)
    // Here we reduce the list to the range of $limit beginning at $start
    $a = 0;
    foreach( $list as $key => $value ) {
      if( $a >= $GLOBALS['start'] && ($a - $GLOBALS['start'] < $GLOBALS['limit'] )) {
        $output_array[$key] = $value;
      }
      $a++;
    }
    $list = $output_array;
  }

  while(list($item,$info) = each($list)) {
    // link to dir / file
    if( is_array( $info )) {
      $abs_item=$info;
      if( extension_loaded('posix')) {
        $user_info = posix_getpwnam( $info['user']);
        $file_info['uid'] = $user_info['uid'];
      }
    } else {
      $abs_item=get_abs_item($dir,$item);
      $file_info = @stat( $abs_item );
    }
    $is_dir = get_is_dir($abs_item);
    $rel_shrname = get_rel_shrname(dirname($abs_item));
    $is_fullaccess = in_fullaccess($rel_shrname);

    if($is_dir) {
      $path = explode($GLOBALS['separator'], $abs_item); // $path[2] is share name in acl
      $path_count = count($path);
    }

    $items['items'][$i]['name']         = $item;
    $items['items'][$i]['is_file']      = get_is_file($abs_item);
    $items['items'][$i]['is_archive']   = $is_fullaccess && ext_isArchive( $item );
    $items['items'][$i]['is_writable']  = $is_fullaccess;
    $items['items'][$i]['is_chmodable'] = $is_fullaccess;
    $items['items'][$i]['is_readable']  = $is_fullaccess;
    $items['items'][$i]['is_deletable'] = $is_fullaccess;
    $items['items'][$i]['is_editable']  = $is_fullaccess && get_is_editable($abs_item);
    $items['items'][$i]['icon']         = _EXT_URL."/images/".get_mime_type($abs_item, "img");
    $items['items'][$i]['size']         = $is_dir?"-":parse_file_size(get_file_size($abs_item));
    // type
    $items['items'][$i]['type']         = get_mime_type($abs_item, "type");
    // modified
    $items['items'][$i]['modified']     = parse_file_date( get_file_date($abs_item) );
    $items['items'][$i]['is_sharename'] = $is_dir?($path_count==3):false;
    // permissions
    $perms = get_file_perms( $abs_item );
    if( strlen($perms)>3) {
      $perms = substr( $perms, 2 );
    }
    $items['items'][$i]['perms'] = $perms. ' ('. parse_file_perms( $perms ).')';

    if( extension_loaded( "posix" )) {
      $user_info = posix_getpwuid( $file_info["uid"] );
      $items['items'][$i]['owner'] = $user_info["name"]. " (".$file_info["uid"].")";
    } else {
      $items['items'][$i]['owner'] = 'n/a';
    }
    if( $is_dir && $sendWhat != 'files') {

      $id = $dir.$GLOBALS['separator'].$item;
      $id = str_replace( $GLOBALS['separator'], '_RRR_', $id );

      $path = explode('_RRR_', $id); // $path[1] is share name in acl
      $path_count = count($path);
      $is_fullaccess = in_fullaccess($path[1]);
      $perms = $is_fullaccess?ext_Lang::msg('aclfullaccess',true):ext_Lang::msg('aclreadonly',true);

      /*
      $qtip ="<strong>".ext_Lang::mime('dir',true)."</strong><br /><strong>".ext_Lang::msg('miscperms',true).":</strong> ".$perms."<br />";
      $qtip.='<strong>'.ext_Lang::msg('miscowner',true).':</strong> '.$items['items'][$i]['owner'];
      */
      $qtip ="<strong>".ext_Lang::mime('dir',true)."</strong><br />";
      $qtip.="<strong>".ext_Lang::msg('miscperms',true).":</strong> ".$perms;
      $dirlist[] = array('text'         => htmlspecialchars($item),
                         'id'           => $id,
                         'qtip'         => $qtip,
                         'allowDrag'    => ($path_count>2),
                         'allowDrop'    => ($path_count<2)?false:$is_fullaccess,
                         'is_writable'  => ($path_count<2)?false:$is_fullaccess,
                         'is_chmodable' => ($path_count<2)?false:$is_fullaccess,
                         'is_readable'  => in_allowed($path[1]),
                         'is_deletable' => ($path_count<2)?false:$is_fullaccess,
                         'is_sharename' => ($path_count==2),
                         'cls'          => 'folder');
    }
    if( !$is_dir && $sendWhat == 'files' || $sendWhat == 'both') {
      $i++;
    }
  }
  while( @ob_end_clean() );

  if( $sendWhat == 'dirs') {
    $result = $dirlist;
  } else {
    $result = $items;
  }
  $json = new Services_JSON();
  echo $json->encode( $result );

  ext_exit();

}
class ext_List extends ext_Action {

  function execAction($dir) { // list directory contents
    global $dir_up, $mosConfig_live_site, $_VERSION;

    $allow=($GLOBALS["permissions"]&01)==01;
    //$admin=((($GLOBALS["permissions"]&04)==04) || (($GLOBALS["permissions"]&02)==02));
    $admin=in_array($GLOBALS['__SESSION']["s_user"],$GLOBALS['admin_users']);

    $dir_up = dirname($dir);
    if($dir_up==".") $dir_up = "";

    if(!get_show_item($dir_up,basename($dir))) {
      ext_Result::sendResult('list', false, $dir." : ".ext_Lang::err( 'accessdir' ));
    } elseif($dir) {
      pass_execute($dir, 'list');
    }

    // Sorting of items
    if($GLOBALS["direction"]=="ASC") {
      $_srt = "no";
    } else {
      $_srt = "yes";
    }

    show_header();
    $scriptTag = '
    <script type="text/javascript" src="'. _EXT_URL . '/fetchscript.php?'
      .'subdir[0]=scripts/extjs/&amp;file[0]=yui-utilities.js'
      .'&amp;subdir[1]=scripts/extjs/&amp;file[1]=ext-yui-adapter.js'
      .'&amp;subdir[2]=scripts/extjs/&amp;file[2]=ext-all.js&amp;gzip=1"></script>
    <script type="text/javascript" src="'. $GLOBALS['script_name'].'?option=com_extplorer&amp;action=include_javascript&amp;file=functions.js"></script>
    <link rel="stylesheet" href="'. _EXT_URL . '/fetchscript.php?subdir[0]=scripts/extjs/css/&file[0]=ext-all.css&amp;subdir[1]=scripts/extjs/css/&file[1]=xtheme-aero.css&amp;gzip=1" />';

    if( defined( 'EXT_STANDALONE' )) {
      $GLOBALS['mainframe']->addCustomHeadTag( $scriptTag );
    } else {
      echo $scriptTag;
    }
    ?>
    <div id="dirtree"></div>
    <div id="dirtree-panel"></div>
    <div id="item-grid"></div>
    <div id="ext_statusbar" class="ext_statusbar"></div>

  <?php
    // That's the main javascript file to build the Layout & App Logic
    include( _EXT_PATH.'/scripts/application.js.php' );

  }

}
?>
