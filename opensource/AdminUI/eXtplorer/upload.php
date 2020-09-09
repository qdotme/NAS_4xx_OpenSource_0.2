<?php
// ensure this file is being included by a parent file
if( !defined( '_JEXEC' ) && !defined( '_VALID_MOS' ) ) die( 'Restricted access' );
/**
 * @version $Id: upload.php,v 1.6 2007/11/08 03:07:59 wiley Exp $
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
*/
/**
 * Uploads file(s)
 *
 */
class ext_Upload extends ext_Action {

  function execAction($dir) {

    if(($GLOBALS["permissions"]&01)!=01) {
      ext_Result::sendResult('upload', false, ext_Lang::err( 'accessfunc' ));
    }

    // Execute
    if(isset($GLOBALS['__POST']["confirm"]) && $GLOBALS['__POST']["confirm"]=="true") {
      pass_execute($dir, 'upload');

      $cnt=count($GLOBALS['__FILES']['userfile']['name']);
      $err=false;
      $err_available=isset($GLOBALS['__FILES']['userfile']['error']);

      // upload files & check for errors
      for($i=0;$i<$cnt;$i++) {
        $errors[$i]=NULL;
        $tmp = $GLOBALS['__FILES']['userfile']['tmp_name'][$i];
        $items[$i] = stripslashes($GLOBALS['__FILES']['userfile']['name'][$i]);
        if($err_available) $up_err = $GLOBALS['__FILES']['userfile']['error'][$i];
        else $up_err=(file_exists($tmp)?0:4);
        $abs = get_abs_item($dir,$items[$i]);

        if($items[$i]=="" || $up_err==4) continue;
        if($up_err==1 || $up_err==2) {
          $errors[$i]=ext_Lang::err( 'miscfilesize' );
          $err=true;  continue;
        }
        if($up_err==3) {
          $errors[$i]=ext_Lang::err( 'miscfilepart' );
          $err=true;  continue;
        }
        if(!@is_uploaded_file($tmp)) {
          $errors[$i]=ext_Lang::err( 'uploadfile' );
          $err=true;  continue;
        }
        if(@file_exists($abs) && empty( $_REQUEST['overwrite_files'])) {
          $errors[$i]=ext_Lang::err( 'itemdoesexist' );
          $err=true;  continue;
        }

        // Upload
        $ok = @$GLOBALS['ext_File']->move_uploaded_file($tmp, $abs);
        if($ok===false || PEAR::isError( $ok )) {
          $errors[$i]=ext_Lang::err( 'uploadfile' );
          if( PEAR::isError( $ok ) ) $errors[$i].= ' ['.$ok->getMessage().']';
          $err=true;  continue;
        }
        else {
          @$GLOBALS['ext_File']->chmod( $abs, 775 );
        }
      }

      if($err) { // there were errors
        $err_msg="";
        for($i=0;$i<$cnt;$i++) {
          if($errors[$i]==NULL) continue;
          $err_msg .= $items[$i]." : ".$errors[$i]."\n";
        }
        ext_Result::sendResult('upload', false, $err_msg);
      }

      ext_Result::sendResult('upload', true, ext_Lang::msg('upload_completed'));
      return;
    }

  ?>
  <div style="width:auto;">
      <div class="x-box-tl"><div class="x-box-tr"><div class="x-box-tc"></div></div></div>
      <div class="x-box-ml"><div class="x-box-mr"><div class="x-box-mc">
          <h3><?php echo ext_Lang::msg('actupload') ?>
          <div style="margin-top:5px; font-size: 12px;">
          <?php echo '<br/>
           '.ext_Lang::msg('max_post_size').' = '. ((get_max_upload_limit() / 1024) / 1024).' MB<br/>'
           .'<br/>';
          ?>
          </div>
          </h3>
        <div id="adminForm">
          <div id="uploadForm"></div>
        </div>
      </div></div></div>
      <div class="x-box-bl"><div class="x-box-br"><div class="x-box-bc"></div></div></div>
  </div>
  <script type="text/javascript">
  // reference:
  //   IE:
  //     http://www.jguru.com/faq/view.jsp?EID=330134
  //   FireFox/Mozilla:
  //     http://www.captain.at/ajax-file-upload.php
  // Note:
  //   Internet Explorer:
  //     1) Open 'Internet Options...' under the 'Tools' menu
  //     2) Select the 'Security' tab
  //     3) Click the 'Custom Level' button
  //     4) Set 'Initialize and script ActiveX controls not marked as safe' to Enable
  //
  //   FireFox/Mozilla:
  //     1) open FireFox/Mozilla
  //     2) open 'about:config' in URL
  //     3) use filter to get 'signed.applets.codebase_principal_support'
  //     4) set 'signed.applets.codebase_principal_support' to true
  function isAllowSecurity() {
    try { // FireFox
      netscape.security.PrivilegeManager.enablePrivilege("UniversalXPConnect");
    } catch (e) {
      try { // Internet Explorer
        var oas = new ActiveXObject("Scripting.FileSystemObject");
      } catch (e) {
        Ext.MessageBox.alert('Security Issue', '<?=bb2html(ext_Lang::msg('upfilesizechk'))?>');
        return false;
      }
    }
    return true;
  }

  function fileSize(filename) {
		try { // FireFox
			/*
				Note from the developer: rodboy
				If mozilla or netscape based browser then the img fileSize property will not work,
				therefore using the following will detect the file size.
			*/

			netscape.security.PrivilegeManager.enablePrivilege("UniversalXPConnect");
			// open the local file
			var file = Components.classes["@mozilla.org/file/local;1"].createInstance(Components.interfaces.nsILocalFile);
			file.initWithPath( filename );
			var stream = Components.classes["@mozilla.org/network/file-input-stream;1"].createInstance(Components.interfaces.nsIFileInputStream);
			stream.init(file,	0x01, 00004, null);
			var bstream =  Components.classes["@mozilla.org/network/buffered-input-stream;1"].getService();
			bstream.QueryInterface(Components.interfaces.nsIBufferedInputStream);
			bstream.init(stream, 1000);
			bstream.QueryInterface(Components.interfaces.nsIInputStream);
			binary = Components.classes["@mozilla.org/binaryinputstream;1"].createInstance(Components.interfaces.nsIBinaryInputStream);
			binary.setInputStream (stream);
			/*
				Note from the developer: rodboy
				return the file size of the image using the binary method.
			*/
			return binary.available();
		} catch (e) {
			/*
				Note from the developer: rodboy
				Either browser is IE, in which case the img onload event will take effect, or
				we'll not be able to detect the file size using the client side javascript.
			*/
			try {
        var oas = new ActiveXObject("Scripting.FileSystemObject");
        var e = oas.getFile(filename);
        return e.size;
			} catch (e) {
			  Ext.MessageBox.alert('Security Issue', '<?=bb2html(ext_Lang::msg('upfilesizechk', true ))?>');
			}
		}
		return -1;
	}

  var simple = new Ext.form.Form({
      labelWidth: 150, // label settings here cascade unless overridden
      url:'<?php echo basename( $GLOBALS['script_name']) ?>',
      fileUpload: true
  });
  simple.add(
    <?php $maxRow = 5; ?>

    <?php
    for($i=0;$i<$maxRow;$i++) {
        echo "new Ext.form.TextField({
            fieldLabel: '".ext_Lang::msg('file', true ).' '.($i+1)."',
            name: 'userfile[$i]',
            width:275,
            inputType: 'file'
        }),";
    }
    ?>

    new Ext.form.Checkbox({
      fieldLabel: '<?php echo ext_Lang::msg('overwrite_files', true ) ?>',
      name: 'overwrite_files',
      checked: true
    })
  );

  simple.addButton('<?php echo ext_Lang::msg( 'btnsave', true ) ?>', function() {
    <?php
      for($i=0;$i<$maxRow;$i++)
        echo "var userfile{$i}=false;";
    ?>
    var uploadSize = 0;

    <?php
      for($i=0;$i<$maxRow;$i++){
        echo "if(simple.findField('userfile[{$i}]').getValue()!=\"\"){";
        echo "userfile{$i}=true;";
        echo "}";
      }
    ?>

    <?php
      $rowAry = array();
      for($i=0;$i<$maxRow;$i++)
        $rowAry[$i] = "userfile{$i}";

      $condition = implode("||",$rowAry);

      // return(javascript) if user doesn't specify any file(s) for upload
      echo "if( !({$condition}) ){";
      echo "Ext.MessageBox.alert('Error!', '".ext_Lang::err('upnoname',true)."');";
      echo "return;";
      echo "}";
    ?>


    <?php
      for($i=0;$i<$maxRow;$i++){
        echo "if(userfile{$i}){";
        // return if it has security issue now
        echo "if((fsize=fileSize(simple.findField('userfile[{$i}]').getValue()))==-1)";
        echo "return;";
        echo "uploadSize += fsize;";
        echo "}";
      }
    ?>

    if(uploadSize >= <?=get_max_upload_limit()?>){
      Ext.MessageBox.alert('Error!', '<?php echo ext_Lang::err( 'upmaximum', true ) ?>');
      return;
    }

    statusBarMessage( '<?php echo ext_Lang::msg( 'upload_processing', true ) ?>', true );
    simple.submit({
        //reset: true,
        reset: false,
        success: function(form, action) {
          datastore.reload();
          statusBarMessage( action.result.message, false, true );
          dialog.destroy();
        },
        failure: function(form, action) {
          if( !action.result ) return;
          Ext.MessageBox.alert('<?php echo ext_Lang::err( 'error', true ) ?>', action.result.error);
          statusBarMessage( action.result.error, false, false );
        },
        scope: simple,
        // add some vars to the request, similar to hidden fields
        params: {requestType: 'xmlhttprequest',
            option: 'com_extplorer',
            action: 'upload',
            dir: datastore.directory,
            confirm: 'true'}
    });
  });
  simple.addButton('<?php echo ext_Lang::msg( 'btncancel', true ) ?>', function() { dialog.destroy(); } );
  simple.render('uploadForm');

  isAllowSecurity();
  </script>
  <?php

  }
}

?>