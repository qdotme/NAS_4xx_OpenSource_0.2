<?php
// ensure this file is being included by a parent file
if( !defined( '_JEXEC' ) && !defined( '_VALID_MOS' ) ) die( 'Restricted access' );
/**
 * @version $Id: copy_move.php,v 1.5 2007/09/11 07:21:36 wiley Exp $
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
 *
 */

/**
 * File/Directory Copy & Move Functions
 */
function copy_move_items($dir) { // copy/move file/dir
  $action = extGetParam( $_REQUEST, 'action' );
  if(($GLOBALS["permissions"]&01)!=01){
    ext_Result::sendResult( $action, false, ext_Lang::err( 'accessfunc' ));
  }

  // Vars
  $first = $GLOBALS['__POST']["first"];
  if($first=="y") $new_dir=$dir;
  else $new_dir = stripslashes($GLOBALS['__POST']["new_dir"]);
  if($new_dir==".") $new_dir="";
  $cnt=count($GLOBALS['__POST']["selitems"]);

  // Do some setup stuff
  ini_set('memory_limit', '128M');
  @set_time_limit( 0 );
  error_reporting( E_ERROR | E_PARSE );

  // DO COPY/MOVE

  // ALL OK?
  if(!@$GLOBALS['ext_File']->file_exists(get_abs_dir($new_dir))) {
    ext_Result::sendResult( $action, false, get_abs_dir($new_dir).": ".ext_Lang::err( 'targetexist' ));
  }
  if(!get_show_item($new_dir,"")) {
    ext_Result::sendResult( $action, false, $new_dir.": ".ext_Lang::err( 'accesstarget' ));
  }
  if(!down_home(get_abs_dir($new_dir))) {
    ext_Result::sendResult( $action, false, $new_dir.": ".ext_Lang::err( 'targetabovehome' ));
  }
  pass_execute($new_dir, $action, $new_dir.": ".ext_Lang::err( 'accesstarget' ));

  // copy / move files
  $err=false;
  for($i=0;$i<$cnt;++$i) {
    $tmp = basename(stripslashes($GLOBALS['__POST']["selitems"][$i]));
    $new = basename(stripslashes($GLOBALS['__POST']["selitems"][$i]));

    $abs_item = get_abs_item($dir,$tmp);
    $abs_new_item = get_abs_item($new_dir,$new);

    $items[$i] = $tmp;

    // Check
    if($new=="") {
      $error[$i]= ext_Lang::err( 'miscnoname' );
      $err=true;  continue;
    }
    if(!@$GLOBALS['ext_File']->file_exists($abs_item)) {
      $error[$i]= ext_Lang::err( 'itemexist' );
      $err=true;  continue;
    }
    if(!get_show_item($dir, $tmp)) {
      $error[$i]= ext_Lang::err( 'accessitem' );
      $err=true;  continue;
    }
    if(@$GLOBALS['ext_File']->file_exists($abs_new_item)) {
      $error[$i]= ext_Lang::err( 'targetdoesexist' );
      $err=true;  continue;
    }

    // Copy / Move
    if($action=="copy") {
      if(@is_link($abs_item) || get_is_file($abs_item)) {
        // check file-exists to avoid error with 0-size files (PHP 4.3.0)
        $ok=@$GLOBALS['ext_File']->copy($abs_item ,$abs_new_item); //||@file_exists($abs_new_item);
      }
      elseif(@get_is_dir($abs_item)) {
        $ok=$GLOBALS['ext_File']->copy_dir($abs_item, $abs_new_item);
      }
    }
    else {
      if(is_dir($abs_item)) {
        @system("/bin/mv \"$abs_item\" \"$abs_new_item\"", $retval);
        $ok=(is_numeric($retval)&&$retval==0)?true:false;
      } else {
        $ok=@$GLOBALS['ext_File']->rename($abs_item,$abs_new_item);
      }
    }

    if($ok===false || PEAR::isError( $ok ) ) {
      $error[$i]=($action=="copy"?ext_Lang::err( 'copyitem' ):ext_Lang::err( 'moveitem' ));
      if( PEAR::isError( $ok ) ) {
        $error[$i].= ' ['.$ok->getMessage().']';
      }
      $err=true;  continue;
    }

    $error[$i]=NULL;
  }

  if($err) {      // there were errors
    $err_msg="";
    for($i=0;$i<$cnt;++$i) {
      if($error[$i]==NULL) continue;

      $err_msg .= $items[$i]." : ".$error[$i]."\n";
    }
    ext_Result::sendResult( $action, false, $err_msg);
  }

  ext_Result::sendResult($action, true, ($action=='copy')?ext_Lang::msg('copy_completed'):ext_Lang::msg('move_completed'));
}
//------------------------------------------------------------------------------
?>
