/*
 * 09/03/2006 - 19:04:30
 * 
 * Ajax progress bar
 * require Prototype Javascript library!
 *
 * Copyright (C) 2006 Michele Ferretti
 * michele.ferretti@gmail.com
 * http://www.blackbirdblog.it
 * 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 *
 */
var instanceNo = 0;
var AjaxProgressBar = Class.create();
AjaxProgressBar.prototype = {
	initialize: function(session, progressElementId, initProperties){
    this.session = session;
    this.progressElementId = document.getElementById(progressElementId);   

    this.frequency = 10;
		if( initProperties != null ){
      this.reload_page = initProperties["reload_page"];
		} else {
      this.reload_page = 'system_firmware_automated.php?lang=en';      
    }
    
    this.id = "AjaxWGAQuery"+ this.session;
		window[this.id] = this;
    this.updatePage = true;
	},
	setValue: function(value){
    this.progressElementId.innerHTML = value+'%';
  },
	start: function(){ this.update(); },
	update: function(){
		var upThis = this;
		var ajax = new Ajax.Request('fw_dn_query.php?id='+this.session, {
			method: "get",
			onSuccess: function(res){ upThis.updateSuccess(res); },
			onFailure: function(res){
        alert("Error: "+ res.status +" "+ res.statusText);
      }
		});
	},
	updateSuccess: function(res){
		var percent = parseInt(res.responseXML.getElementsByTagName("value")[0].firstChild.nodeValue);
		this.setValue(percent);
    
    if(percent>=100){
      setTimeout("window.location.href = '"+this.reload_page+"'", 2000);
    } else {
      setTimeout("window."+ this.id +".update()", this.frequency*1000);
    }
	}
}
