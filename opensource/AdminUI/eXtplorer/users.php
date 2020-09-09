<?php
// ensure this file is being included by a parent file
if( !defined( '_JEXEC' ) && !defined( '_VALID_MOS' ) ) die( 'Restricted access' );
/**
 * @version $Id: users.php,v 1.5.10.1 2009/02/05 02:54:42 wiley Exp $
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
function load_users() {
  require _EXT_PATH."/config/.htusers.php";
  require _EXT_PATH."/../admin/.htadmin.php";
}
//------------------------------------------------------------------------------
function save_users() {
  $cnt=count($GLOBALS["users"]);
  if($cnt>0) sort($GLOBALS["users"]);

  // Make PHP-File
  $content='<?php
  /** ensure this file is being included by a parent file */
  defined( "_VALID_MOS" ) or die( "Direct Access to this location is not allowed." );
  $GLOBALS["users"]=array(';
  for($i=0;$i<$cnt;++$i) {
    // if($GLOBALS["users"][6]&4==4) $GLOBALS["users"][6]=7;  // If admin, all permissions
    $content.="\r\n\tarray(\"".$GLOBALS["users"][$i][0].'","'.
      $GLOBALS["users"][$i][1].'","'.$GLOBALS["users"][$i][2].'","'.$GLOBALS["users"][$i][3].'",'.
      $GLOBALS["users"][$i][4].',"'.$GLOBALS["users"][$i][5].'",'.$GLOBALS["users"][$i][6].','.
      $GLOBALS["users"][$i][7].'),';
  }
  $content.="\r\n); \r\n?>";

  // Write to File
  if( !is_writable(_EXT_PATH."/config/.htusers.php") && !chmod( _EXT_PATH."/config/.htusers.php", 0644 ) ) {
    return false;
  }
  file_put_contents( _EXT_PATH."/config/.htusers.php", $content);

  return true;
}
//------------------------------------------------------------------------------
function find_user($user,$pass) {
  $cnt=count($GLOBALS["users"]);
  for($i=0;$i<$cnt;++$i) {
    if($user==$GLOBALS["users"][$i][0]) {
      if($pass==NULL || ($pass==$GLOBALS["users"][$i][1] &&
        $GLOBALS["users"][$i][7])) {
        return $GLOBALS["users"][$i];
      }
      /*
      elseif ($GLOBALS["users"][$i][1] == "ThisIsDomainUser") { // is ADS user?
        // authorize ADS user if any
        $p_pass = $GLOBALS['__POST']["p_pass"];
        $p_user = $user;

        if (strpos($user, "\\") !== FALSE) {
          $ads_user = explode("\\", $user);
          $p_user   = $ads_user[1];
        }

        @exec("/usr/bin/wbinfo --sequence >/dev/null 2>&1");
        @exec("/usr/bin/net user -U \"$p_user%$p_pass\"", $output, $retval);

        if (is_numeric($retval) && $retval == 0) {
          return $GLOBALS["users"][$i];
        }
      }
      */
    }
  }

  return NULL;
}
//------------------------------------------------------------------------------
function activate_user($user,$pass) {
  $data=find_user($user,$pass);
  if($data==NULL) return false;

  // Set Login
  $GLOBALS['__SESSION']["s_user"] = $data[0];
  $GLOBALS['__SESSION']["s_pass"] = $data[1];
  $GLOBALS["home_dir"]            = str_replace( '\\', '/', $data[2] );
  $GLOBALS["home_url"]            = $data[3];
  $GLOBALS["show_hidden"]         = $data[4];
  $GLOBALS["no_access"]           = $data[5];
  $GLOBALS["permissions"]         = $data[6];

  // Get Share Access Control List
  $GLOBALS["acl_shares"]     = $GLOBALS['shares'][$data[0]];
  $GLOBALS["acl_readonly"]   = $GLOBALS["acl_shares"][0];
  $GLOBALS["acl_fullaccess"] = $GLOBALS["acl_shares"][1];

  return true;
}
//------------------------------------------------------------------------------
?>
