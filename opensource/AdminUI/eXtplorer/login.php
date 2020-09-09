<?php
// ensure this file is being included by a parent file
if( !defined( '_JEXEC' ) && !defined( '_VALID_MOS' ) ) die( 'Restricted access' );
/**
 * @version $Id: login.php,v 1.3 2007/09/07 04:46:04 wiley Exp $
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
 * User Authentication Functions
 * (currently not used)
 */

//------------------------------------------------------------------------------
require_once _EXT_PATH."/include/users.php";
require_once _EXT_PATH."/include/shares.php";
load_users();
load_shares();
//------------------------------------------------------------------------------

$GLOBALS['__SESSION']=&$_SESSION;

//------------------------------------------------------------------------------
function login() {
  if(!empty($GLOBALS['__SESSION']["s_user"])) {
    if(!activate_user($GLOBALS['__SESSION']["s_user"],$GLOBALS['__SESSION']["s_pass"])) {
      logout();
    }
  } else {
    @header("Location: {$GLOBALS['http_host']}/?lang=".$GLOBALS["language"]);
  }
}
//------------------------------------------------------------------------------
function logout() {
  @session_destroy();
  @session_write_close();
  @header("Location: {$GLOBALS['http_host']}/?lang=".$GLOBALS["language"]);
}
//------------------------------------------------------------------------------
/**
 * Returns an IP- and BrowserID- based Session ID
 *
 * @param string $id
 * @return string
 */
function get_session_id( $id=null ) {
  return extMakePassword( 32 );
}
?>
