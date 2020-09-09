<?php
// ensure this file is being included by a parent file
if( !defined( '_JEXEC' ) && !defined( '_VALID_MOS' ) ) die( 'Restricted access' );
/**
 * @version $Id: mkitem.php,v 1.6 2007/11/07 09:45:32 wiley Exp $
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
 */
/**
 * Allows to create dirs, files and symlinks on a server
 *
 */
class ext_Mkitem extends ext_Action {

  function execAction($dir) {    // make new directory or file
    if(($GLOBALS["permissions"]&01)!=01) ext_Result::sendResult( 'mkitem', false, ext_Lang::err( 'accessfunc' ));

    if( extGetParam($_POST,'confirm') == 'true') {
      pass_execute($dir, 'mkitem');

      $mkname=$GLOBALS['__POST']["mkname"];
      $mktype=$GLOBALS['__POST']["mktype"];

      $mkname=basename(stripslashes($mkname));
      if($mkname=="") ext_Result::sendResult( 'mkitem', false,  ext_Lang::err( 'miscnoname' ) );

      $new = get_abs_item($dir,$mkname);

      if(@$GLOBALS['ext_File']->file_exists($new)) {
        ext_Result::sendResult( 'mkitem', false, $mkname.": ".ext_Lang::err( 'itemdoesexist' ));
      }
      $err = print_r( $_POST, true );
      if($mktype=="dir") {
        $ok=@$GLOBALS['ext_File']->mkdir($new, 0777);
        $ok=@chgrp($new, 'jewab');
        $err=ext_Lang::err( 'createdir' );
      } elseif($mktype == 'file') {
        $ok=@$GLOBALS['ext_File']->mkfile($new);
        $err=ext_Lang::err( 'createfile' );
      }

      if($ok==false || PEAR::isError( $ok )) {
        if( PEAR::isError( $ok ) ) $err.= $ok->getMessage();
        ext_Result::sendResult( 'mkitem', false, $err);
      }
      ext_Result::sendResult( 'mkitem', true, sprintf(ext_Lang::msg('mkitem_completed'),$mkname) );
      return;
    }
  ?>
    <div>
      <div class="x-box-tl"><div class="x-box-tr"><div class="x-box-tc"></div></div></div>
      <div class="x-box-ml"><div class="x-box-mr"><div class="x-box-mc">

          <h3 style="margin-bottom:5px;"><?php echo ext_Lang::msg( 'actmkitem') ?></h3>
          <div id="adminForm">

          </div>
      </div></div></div>
      <div class="x-box-bl"><div class="x-box-br"><div class="x-box-bc"></div></div></div>
  </div>
  <script type="text/javascript">
  var mktypes = new Ext.data.SimpleStore({
      fields: ['mktype', 'type'],
      data :  [
          ['file', '<?php echo ext_Lang::mime( 'file', true ) ?>'],
          ['dir', '<?php echo ext_Lang::mime( 'dir', true ) ?>']
          ]
  });
  var simple = new Ext.form.Form({
      labelWidth: 125, // label settings here cascade unless overridden
      url:'<?php echo basename($GLOBALS['script_name']) ?>'
  });
  simple.add(
      new Ext.form.TextField({
          fieldLabel: '<?php echo ext_Lang::msg( 'nameheader', true ) ?>',
          name: 'mkname',
          width:175,
          allowBlank:false
      }),
      new Ext.form.ComboBox({
        fieldLabel: 'Type',
        store: mktypes,
        displayField:'type',
        valueField: 'mktype',
        value: 'file',
        hiddenName: 'mktype',
        disableKeyFilter: true,
        editable: false,
        triggerAction: 'all',
        mode: 'local',
        allowBlank: false,
        selectOnFocus:true
      })
  );

  simple.addButton('<?php echo ext_Lang::msg( 'btncreate', true ) ?>', function() {
    if(simple.findField('mkname').getValue()==""){
      Ext.MessageBox.alert('Error!', '<?php echo ext_Lang::err( 'miscnoname', true ) ?>');
      return;
    }

    statusBarMessage( 'Please wait...', true );
    simple.submit({
        //reset: true,
        reset: false,
        success: function(form, action) {
          statusBarMessage( action.result.message, false, true );
          try{
            dirTree.getSelectionModel().getSelectedNode().reload();
          } catch(e) {}
          datastore.reload();
          dialog.destroy();
        },
        failure: function(form, action) {
          if( !action.result ) return;
          Ext.MessageBox.alert('Error!', action.result.error);
          statusBarMessage( action.result.error, false, true );
        },
        scope: simple,
        // add some vars to the request, similar to hidden fields
        params: {requestType: 'xmlhttprequest',
            option: 'com_extplorer',
            action: 'mkitem',
            dir: datastore.directory,
            confirm: 'true'}
    })
  });
  simple.addButton('<?php echo ext_Lang::msg( 'btncancel', true ) ?>', function() { dialog.destroy(); } );
  simple.render('adminForm');
  simple.findField('mkname').focus();
  </script>
  <?php
  }
}

//------------------------------------------------------------------------------
?>
