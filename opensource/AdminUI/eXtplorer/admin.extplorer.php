<?php
// ensure this file is being included by a parent file
if( !defined( '_JEXEC' ) && !defined( '_VALID_MOS' ) ) die( 'Restricted access' );
/**
 * MAIN FILE! (formerly known as index.php)
 *
 * @version $Id: admin.extplorer.php,v 1.6 2007/09/14 06:20:00 wiley Exp $
 *
 * @package eXtplorer
 * @copyright soeren 2007
 * @author The eXtplorer project (http://sourceforge.net/projects/extplorer)
 * @author The  The QuiX project (http://quixplorer.sourceforge.net)
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
 * This is a component with full access to the filesystem of your joomla Site
 * I wouldn't recommend to let in Managers
 * allowed: Superadministrator
**/
//------------------------------------------------------------------------------
if( defined( 'E_STRICT' ) ) { // Suppress Strict Standards Warnings
  error_reporting(E_ALL);
}
//------------------------------------------------------------------------------
umask(0002); // Added to make created files/dirs group writable
//------------------------------------------------------------------------------
require_once dirname( __FILE__) . "/include/init.php";  // Init
//------------------------------------------------------------------------------
if( $action == "post" )
  $action = extGetParam( $_REQUEST, "do_action" );
elseif( empty( $action ))
  $action = "list";

if( $action == 'include_javascript' ) {
    while (@ob_end_clean());
    header("Content-type: application/x-javascript; charset=iso-8859-1");
    include( _EXT_PATH.'/scripts/'.basename(extGetParam($_REQUEST, 'file' )).'.php');
    ext_exit();
}
if( $action != 'show_error') {
  ext_Result::init();
}

if( defined( '_LOGIN_REQUIRED')) return;

// Empty the output buffer if this is a XMLHttpRequest
if( ext_isXHR() ) {
  error_reporting(0);
  while( @ob_end_clean() );
}

if( file_exists( _EXT_PATH . '/include/'. strtolower(basename( $action )) .'.php') ) {
  require_once( _EXT_PATH . '/include/'. strtolower(basename( $action )) .'.php');
}
$classname = 'ext_'.$action;
if( class_exists(strtolower($classname))) {
  $handler = new $classname();
  $handler->execAction( $dir, $item );
} else {

  switch($action) { // Execute actions, which are not in class file

  //------------------------------------------------------------------------------
    // COPY/MOVE FILE(S)/DIR(S)
    case "copy":
    case "move":
      require_once _EXT_PATH ."/include/copy_move.php";
      copy_move_items($dir);
    break;

  //------------------------------------------------------------------------------
    // SEARCH FOR FILE(S)/DIR(S)
    case "search":
      require_once _EXT_PATH ."/include/search.php";
      search_items($dir);
    break;
  //------------------------------------------------------------------------------
    case 'show_error':
      ext_Result::sendResult('', false, '');
      break;
  //------------------------------------------------------------------------------
    // DEFAULT: LIST FILES & DIRS
    case "getdircontents":
        require_once _EXT_PATH . "/include/list.php";
        $requestedDir = stripslashes(str_replace( '_RRR_', $GLOBALS['separator'], extGetParam( $_REQUEST, 'node' )));
        if( empty($requestedDir) || $requestedDir == 'ext_root') {
          $requestedDir = $dir;
        }
        send_dircontents( $requestedDir, extGetParam($_REQUEST,'sendWhat','files') );
        break;
    case 'get_dir_selects':
        echo get_dir_selects( $dir );
        break;
    case 'chdir_event':
        $response = Array('dirselects' => get_dir_selects($dir));
        $json = new Services_JSON();
        echo $json->encode( $response );
        break;
    case 'get_image':
        require_once _EXT_PATH . "/include/view.php";
        ext_View::sendImage( $dir, $item );
    default:
      require_once _EXT_PATH . "/include/list.php";
      ext_List::execAction($dir);
  //------------------------------------------------------------------------------
  }
// end switch-statement
}
//------------------------------------------------------------------------------
// Empty the output buffer if this is a XMLHttpRequest
if( ext_isXHR() ) {
  ext_exit();
}


//------------------------------------------------------------------------------
?>
