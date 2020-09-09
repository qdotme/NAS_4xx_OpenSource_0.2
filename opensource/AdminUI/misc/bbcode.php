<?php
/**
 * @version $Id: bbcode.php,v 1.2 2007/09/05 09:24:51 wiley Exp $
 *
 * @license
 * It was written by WAY2WEB.net
 * It is free for you to use anywhere as long as you provide a link back to
 * www.way2web.net
 *
 * Syntax Sample:
 * [img]http://elouai.com/images/star.gif[/img]
 * [url=http://elouai.com]eLouai[/url]
 * [mail=webmaster@elouai.com]Webmaster[/mail]
 * [size=25]HUGE[/size]
 * [color=red]RED[/color]
 * [b]bold[/b]
 * [i]italic[/i]
 * [u]underline[/u]
 * [s]strike-through[/s]
 * [o]over-line[/o]
 * [center]center[/center]
 * [right]right[/right]
 * [justify]justify justify justify[/justify]
 * [list][*]item[*]item[*]item[/list]
 * [list=1][*]item 1[*]item 1[*]item 1[/list]
 * [list=i][*]item i[*]item i[*]item i[/list]
 * [list=I][*]item I[*]item I[*]item I[/list]
 * [list=a][*]item a[*]item a[*]item a[/list]
 * [list=A][*]item A[*]item A[*]item A[/list]
 * [font=verdana]this is Verdana font[/font]
 * [code]value="123";[/code]
 * [php]$value="123";[/php]
 * [quote]John said yadda yadda yadda[/quote]
 *
 * Usage:
 * <?php include 'bbcode.php'; ?>
 * <?php $htmltext = bb2html($bbtext); ?>
 */

/**
 * A simple FAST parser to convert BBCode to HTML
 *
 */
function bb2html($Text) {
  // Replace any html brackets with HTML Entities to prevent executing HTML or script
  // Don't use strip_tags here because it breaks [url] search by replacing & with amp
  $Text = str_replace("<", "&lt;", $Text);
  $Text = str_replace(">", "&gt;", $Text);

  // Convert new line chars to html <br /> tags
  $Text = nl2br($Text);
  // New line <br /> via [br]
  $Text = str_replace("[br]", "<br />", $Text);

  // Set up the parameters for a URL search string
  $URLSearchString = " a-zA-Z0-9\:\/\-\?\&\.\=\_\~\#\'";
  // Set up the parameters for a MAIL search string
  $MAILSearchString = $URLSearchString . " a-zA-Z0-9\.@";

  // New line <br /> via [br]
  $Text = preg_replace("(\[b\](.+?)\[\/b])is",
            '<span style="font-weight: bold;">$1</span>',$Text);

  // Perform URL Search
  $Text = preg_replace("/\[url\]([$URLSearchString]*)\[\/url\]/", '<a href="$1" target="_blank">$1</a>', $Text);
  $Text = preg_replace("(\[url\=([$URLSearchString]*)\](.+?)\[/url\])", '<a href="$1" target="_blank">$2</a>', $Text);

  // Perform MAIL Search
  $Text = preg_replace("(\[mail\]([$MAILSearchString]*)\[/mail\])", '<a href="mailto:$1">$1</a>', $Text);
  $Text = preg_replace("/\[mail\=([$MAILSearchString]*)\](.+?)\[\/mail\]/", '<a href="mailto:$1">$2</a>', $Text);

  // Check for bold text
  $Text = preg_replace("(\[b\](.+?)\[\/b])is",
            '<span style="font-weight: bold;">$1</span>',$Text);

  // Check for Italics text
  $Text = preg_replace("(\[i\](.+?)\[\/i\])is",
            '<span style="font-style: italic;">$1</span>',$Text);

  // Check for Underline text
  $Text = preg_replace("(\[u\](.+?)\[\/u\])is",
            '<span style="text-decoration: underline;">$1</span>',$Text);

  // Check for strike-through text
  $Text = preg_replace("(\[s\](.+?)\[\/s\])is",
            '<span style="text-decoration: line-through;">$1</span>',$Text);

  // Check for over-line text
  $Text = preg_replace("(\[o\](.+?)\[\/o\])is",
            '<span style="text-decoration: overline;">$1</span>',$Text);

  // Align center
  $Text = preg_replace("(\[center\](.+?)\[\/center])is",
            '<div style="text-align: center">$1</div>',$Text);

  // Align right
  $Text = preg_replace("(\[right\](.+?)\[\/right])is",
            '<div style="text-align: right">$1</div>',$Text);

  // Justify text
  $Text = preg_replace("(\[justify\](.+?)\[\/justify])is",
            '<div style="text-align: justify">$1</div>',$Text);

  // Check for colored text
  $Text = preg_replace("(\[color=(.+?)\](.+?)\[\/color\])is",
            '<span style="color: $1">$2</span>',$Text);

  // Check for sized text
  $Text = preg_replace("(\[size=(.+?)\](.+?)\[\/size\])is",
            '<span style="font-size: $1px">$2</span>',$Text);

  // Check for list text
  $Text = preg_replace("/\[list\](.+?)\[\/list\]/is",
            '<ul style="list-style-type: disc; list-style-position: inside;">$1</ul>' ,$Text);
  $Text = preg_replace("/\[list=1\](.+?)\[\/list\]/is",
            '<ul style="list-style-type: decimal; list-style-position: inside;">$1</ul>' ,$Text);
  $Text = preg_replace("/\[list=i\](.+?)\[\/list\]/s",
            '<ul style="list-style-type: lower-roman; list-style-position: inside;">$1</ul>' ,$Text);
  $Text = preg_replace("/\[list=I\](.+?)\[\/list\]/s",
            '<ul style="list-style-type: upper-roman; list-style-position: inside;">$1</ul>' ,$Text);
  $Text = preg_replace("/\[list=a\](.+?)\[\/list\]/s",
            '<ul style="list-style-type: lower-alpha; list-style-position: inside;">$1</ul>' ,$Text);
  $Text = preg_replace("/\[list=A\](.+?)\[\/list\]/s",
            '<ul style="list-style-type: upper-alpha; list-style-position: inside;">$1</ul>' ,$Text);
  $Text = str_replace("[*]", "<li>", $Text);

  // Check for font change text
  $Text = preg_replace("(\[font=(.+?)\](.+?)\[\/font\])","<span style=\"font-family: $1;\">$2</span>",$Text);

  // Declare the CSS for [code], [php] and [quote]
  $QuoteCodeHeaderStyle = 'font-family: Verdana, Arial, Helvetica, Sans-Serif; '
                          .'font-size: 12px; '
                          .'font-weight: bold; ';

  $CodeBodyStyle = 'background-color: #FFFFFF; '
                   .'font-family: Courier New, Courier, Mono; '
                   .'font-size: 12px; '
                   .'color: #006600; '
                   .'border: 1px solid #BFBFBF; ';

  $QuoteBodyStyle = 'background-color: #FFFFFF; '
                    .'font-family: Courier New, Courier, Mono; '
                    .'font-size: 12px; '
                    .'color: #660002; '
                    .'border: 1px solid #BFBFBF; ';

  // Declare the format for [code] layout
  $CodeLayout = "<table width=\"90%\" border=\"0\" align=\"center\" cellpadding=\"0\" cellspacing=\"0\">
                  <tr>
                    <td style=\"$QuoteCodeHeaderStyle\"> Code:</td>
                  </tr>
                  <tr>
                    <td style=\"$CodeBodyStyle\">$1</td>
                  </tr>
                </table>";
  // Check for [code] text
  $Text = preg_replace("/\[code\](.+?)\[\/code\]/is","$CodeLayout", $Text);
  // Declare the format for [php] layout
  $phpLayout = "<table width=\"90%\" border=\"0\" align=\"center\" cellpadding=\"0\" cellspacing=\"0\">
                  <tr>
                    <td style=\"$QuoteCodeHeaderStyle\">PHP Code:</td>
                  </tr>
                  <tr>
                    <td style=\"$CodeBodyStyle\">$1</td>
                  </tr>
                </table>";
  // Check for [php] text
  $Text = preg_replace("/\[php\](.+?)\[\/php\]/is",$phpLayout, $Text);

  // Declare the format for [quote] layout
  $QuoteLayout = "<table width=\"90%\" border=\"0\" align=\"center\" cellpadding=\"0\" cellspacing=\"0\">
                    <tr>
                      <td style=\"$QuoteCodeHeaderStyle\"> Quote:</td>
                    </tr>
                    <tr>
                      <td style=\"$QuoteBodyStyle\">$1</td>
                    </tr>
                  </table>";

  // Check for [quote] text
  $Text = preg_replace("/\[quote\](.+?)\[\/quote\]/is","$QuoteLayout", $Text);

  // Images
  // [img]pathtoimage[/img]
  $Text = preg_replace("/\[img\](.+?)\[\/img\]/", '<img src="$1">', $Text);

  // [img=widthxheight]image source[/img]
  $Text = preg_replace("/\[img\=([0-9]*)x([0-9]*)\](.+?)\[\/img\]/", '<img src="$3" height="$2" width="$1">', $Text);

  return $Text;
}
?>