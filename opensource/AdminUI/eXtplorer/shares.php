<?php
// ensure this file is being included by a parent file
if( !defined( '_JEXEC' ) && !defined( '_VALID_MOS' ) ) die( 'Restricted access' );
/**
 * @version $Id: shares.php,v 1.2 2007/09/05 09:24:51 wiley Exp $
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
 * Administrative Functions regarding users
 */
function load_shares() {
  require _EXT_PATH."/config/.htshares.php";
}
//------------------------------------------------------------------------------
function get_rel_shrname($dir){
  $homedir=realpath($GLOBALS["home_dir"]);
  $abs_dir=get_abs_dir($dir);

  if ($abs_dir == $homedir) {
    return "ext_root";
  }

  if ($dir!="" && @stristr( $abs_dir, $homedir )) {
    if (($rel_dir = substr($abs_dir,strlen($homedir)))!==false) {
      $path = explode($GLOBALS['separator'],$rel_dir);
      return $path[1];
    }
  }
  return false;
}
//------------------------------------------------------------------------------
function allowed_shares(){
  return array_unique(array_merge(readonly_shares(),fullaccess_shares()));
}
//------------------------------------------------------------------------------
function readonly_shares(){
  if (!is_array($GLOBALS["acl_readonly"])) {
    return array();
  }
  return $GLOBALS["acl_readonly"];
}
//------------------------------------------------------------------------------
function fullaccess_shares(){
  if (!is_array($GLOBALS["acl_fullaccess"])) {
    return array();
  }
  return $GLOBALS["acl_fullaccess"];
}
//------------------------------------------------------------------------------
function in_allowed($shrname){
  return in_array($shrname, allowed_shares());
}
//------------------------------------------------------------------------------
function in_readonly($shrname){
  return in_array($shrname, readonly_shares());
}
//------------------------------------------------------------------------------
function in_fullaccess($shrname){
  return in_array($shrname, fullaccess_shares());
}
//------------------------------------------------------------------------------
function is_available($shrname){
  if (in_allowed($shrname)) {
    $homedir=realpath($GLOBALS["home_dir"]);
    @system("/bin/cat /proc/mounts | grep '{$homedir}/{$shrname}' >/dev/null 2>&1", $retval);
    if (is_numeric($retval) && $retval == 0) {
      return true;
    }
  }
  return false;
}
//------------------------------------------------------------------------------
function pass_execute($dir, $action, $message=null){
  if (($rel_shrname = get_rel_shrname($dir))===false) {
    ext_Result::sendResult($action, false, ($message)?$message:ext_Lang::err( 'accessfunc' ));
  }
  if (!is_available($rel_shrname)||!in_fullaccess($rel_shrname)) {
    ext_Result::sendResult($action, false, ($message)?$message:ext_Lang::err( 'accessfunc' ));
  }
}
//------------------------------------------------------------------------------
?>
