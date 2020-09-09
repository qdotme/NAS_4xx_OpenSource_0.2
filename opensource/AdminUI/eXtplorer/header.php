<?php
// ensure this file is being included by a parent file
if( !defined( '_JEXEC' ) && !defined( '_VALID_MOS' ) ) die( 'Restricted access' );
/**
 * @version $Id: header.php,v 1.5 2007/09/12 03:31:03 wiley Exp $
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
 * This is the file, which prints the header row with the Logo
 */
function show_header() {
  require_once _EXT_PATH . '/../admin/homepage_url.inc';

  $url = str_replace( '&dir=', '&ignore=', $_SERVER['REQUEST_URI'] );
  echo "<link rel=\"stylesheet\" href=\""._EXT_URL."/style/style.css\" type=\"text/css\" />\n";
  echo "<div id=\"ext_header\">\n";
  echo "<table border=\"0\" width=\"100%\" cellspacing=\"0\" cellpadding=\"5\">\n";
  echo "<tr><td>";
  echo "<a href=\"".$_homepage."\" target=\"_blank\" title=\"\">";
  echo "<img src=\"/admin/image/small_logo.jpg\" alt=\"\" width=\"200\" height=\"70\" border=\"0\" />";
  echo "</a></td>";
  echo '</tr></table>';
  echo '</div>';
}
//------------------------------------------------------------------------------
?>
