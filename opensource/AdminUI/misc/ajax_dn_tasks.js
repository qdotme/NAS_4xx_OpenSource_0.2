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

var AjaxWGAQuery = Class.create();
AjaxWGAQuery.prototype = {
	initialize: function(session, elementIDArray, initProperties){
    this.session = session;
    this.element = elementIDArray;    

    if( initProperties != null ){
			this.frequency = initProperties["frequency"];
      this.reload_page = initProperties["reload_page"];
      this.debug = (initProperties["debug"]!=undefined)?initProperties["debug"]:false;
		} else { this.frequency = 20; }

		this.id = "AjaxWGAQuery"+ this.session;
		window[this.id] = this;
    this.updatePage = true;
    
    this.xml = null
    this.data = null;
	},
	start: function(){ this.update(); },
	update: function(){
    this.xml = new JKL.ParseXML("dn_query.php?id="+this.session);
    this.data = this.xml.parse();
    
    if(this.xml != undefined && this.data != undefined) {
      this.updateSuccess();
    }
    
    /*
		var upThis = this;
		var ajax = new Ajax.Request("dn_query.php?id="+this.session, {
			method: "get",
			onSuccess: function(res){ upThis.updateSuccess(res); },
			onFailure: function(res){ alert("Error: "+ res.status +" "+ res.statusText);}
		});		
    */
	},
  alertMessage: function(msg){
    if(msg!="") { alert(msg); }
    
    if(!this.updatePage) { // error occurs
      reloadThisPageNow();
    }
  },
	updateSuccess: function(){
    if(this.data.wixnas==undefined ||
        parseInt(this.data.wixnas.running_count) != this.element.length){
      setTimeout("window."+ this.id +".alertMessage('Refresh this page now. (ERROR: undefined dNode)')", 30000);
      this.updatePage = false; return;
    }
    
    for(var i=0;i<this.element.length;i++){
      var dNode = eval("this.data.wixnas."+this.element[i]);
      
      this.sizeElement = document.getElementById("size_"+this.element[i]);
      this.sizeElement.innerHTML = (dNode.size!=undefined)?dNode.size:'';
      
      this.downloadedElement = document.getElementById("downloaded_"+this.element[i]);
      this.downloadedElement.innerHTML = (dNode.downloaded_size!=undefined)?dNode.downloaded_size:'';
      wgaid2dnsize.set(this.element[i], this.downloadedElement.innerHTML);
      
      this.speedElement = document.getElementById("speed_"+this.element[i]);
      this.speedElement.innerHTML = (dNode.speed!=undefined)?dNode.speed:'';
      wgaid2speed.set(this.element[i], this.speedElement.innerHTML);
      
      // debug
      if(this.debug){
        alert(this.element[i]+":\n"+
              "url: "+dNode.url+"\n"+
              "username_password_required: "+dNode.username_password_required+"\n"+
              "username: "+dNode.username+"\n"+
              "password: "+dNode.password+"\n"+
              "location: "+dNode.location+"\n"+
              "save_as: "+dNode.save_as+"\n"+
              "size: "+dNode.size+"\n"+
              "downloaded_size: "+dNode.downloaded_size+"\n"+
              "timestamp: "+dNode.timestamp+"\n"+
              "arrangement: "+dNode.arrangement);
      }
    }
    
    if(this.updatePage){
			setTimeout("window."+ this.id +".update()", this.frequency*1000);
		}
	}
}
