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
	initialize: function(ElementSize, initProperties){
		//this.barElement = document.getElementById(barElementId);
		//this.test_barElement = new Array();
		//this.barElement[0] = document.getElementById(barElementId+"1");
		//this.barElement[1] = document.getElementById(barElementId+"2");
		this.reload_page       = "index.php";
		this.ElementSize       = ElementSize;
		this.percentageElement = new Array();
		this.dlElement         = new Array();
		this.ulElement         = new Array();
		this.estimatedElement  = new Array();
		this.healthElement     = new Array();
		this.tdownloadElement  = document.getElementById("tdownload");
		this.tuploadElement    = document.getElementById("tupload");
		for (var i = 0; i < this.ElementSize; i++) {
		  this.percentageElement[i] = document.getElementById("percentage"+(i+1));
		  this.dlElement[i]         = document.getElementById("dl"+(i+1));
		  this.ulElement[i]         = document.getElementById("ul"+(i+1));
		  this.estimatedElement[i]  = document.getElementById("estimated"+(i+1));
		  this.healthElement[i]     = document.getElementById("health"+(i+1));
		}

		if( initProperties != null ){
			this.frequency = initProperties["frequency"];
		} else { this.frequency = 20; }

		this.isFinish = false;
		this.id = "AjaxProgressBar"+ instanceNo++;
		window[this.id] = this;
	},
	start: function(){ this.update(); },
	update: function(){
	  if (this.ElementSize > 0) {
	    var upThis = this;
  		var ajax = new Ajax.Request("ctcs_inquire.php?query=WCTALLSTATUS", {
  			method: "get",
  			onSuccess: function(res){ upThis.updateSuccess(res); },
  			onFailure: function(res){ alert("Error: "+ res.status +" "+ res.statusText);}
  		});
  		if( !this.isFinish ){
  			setTimeout("window."+ this.id +".update()", this.frequency*1000);
  		}
	  } else {
	   setTimeout("window.location='"+this.reload_page+"'", 60000); // refresh page after 60 seconds
	  }
	},
	updateSuccess: function(res){
		var elemsCTSTATUS   = res.responseXML.getElementsByTagName("CTSTATUS");
		var elemsCTDETAIL   = res.responseXML.getElementsByTagName("CTDETAIL");
		var n_elemsCTSTATUS = elemsCTSTATUS.length;
    var n_elemsCTDETAIL = elemsCTDETAIL.length;

    if (this.ElementSize != n_elemsCTSTATUS || n_elemsCTSTATUS != n_elemsCTDETAIL) {
      setTimeout("window.location='"+this.reload_page+"'", 30000); // refresh page after 30 seconds
    }

    var tupload = 0, tdownload = 0;
    for (var i = 0; i < n_elemsCTSTATUS; i++) {
      var n_have       = parseInt(elemsCTSTATUS[i].childNodes[2].firstChild.nodeValue,10);
      var n_total      = parseInt(elemsCTSTATUS[i].childNodes[3].firstChild.nodeValue,10);
      var n_avail      = parseInt(elemsCTSTATUS[i].childNodes[4].firstChild.nodeValue,10);
      var dl_rate      = parseInt(elemsCTSTATUS[i].childNodes[5].firstChild.nodeValue,10);
      var ul_rate      = parseInt(elemsCTSTATUS[i].childNodes[6].firstChild.nodeValue,10);
      var torrent_size = parseInt(elemsCTDETAIL[i].childNodes[0].firstChild.nodeValue,10);

      var left_seconds = Math.floor((torrent_size*(1-n_have/n_total))/dl_rate);
      var left_time    = "99:99:99";
      if (left_seconds > 0) {
        var hours   = Math.floor(left_seconds/3600);
        var minutes = Math.floor((left_seconds-hours*3600)/60);
        var seconds = Math.floor(left_seconds-hours*3600-minutes*60);
        if (hours <= 99) {
          left_time = ((hours<=9)?"0"+hours:hours)+
                      ":"+
                      ((minutes<=9)?"0"+minutes:minutes)+
                      ":"+
                      ((seconds<=9)?"0"+seconds:seconds);
        }
      }

      if (dl_rate==0 && ul_rate==0) {
		    document.images["simg"+(i+1)].src=img_standby.src;
		  } else if (ul_rate>0 && n_have>=n_total) {
		    document.images["simg"+(i+1)].src=img_upload.src;
		  } else if (dl_rate>0 && n_have<n_total) {
		    document.images["simg"+(i+1)].src=img_download.src;
		  }

      this.percentageElement[i].innerHTML = Math.floor((100*n_have)/n_total)+"%";
      this.dlElement[i].innerHTML         = Math.floor(dl_rate/1024)+" KB/s";
		  this.ulElement[i].innerHTML         = Math.floor(ul_rate/1024)+" KB/s";
		  this.estimatedElement[i].innerHTML  = (left_seconds<=0)?"99:99:99":left_time;
		  this.healthElement[i].innerHTML     = Math.floor((100*n_avail)/n_total)+"%";

      tdownload += dl_rate;
		  tupload   += ul_rate;
		  this.tdownloadElement.innerHTML = Math.floor(tdownload/1024)+" KB/s";
		  this.tuploadElement.innerHTML   = Math.floor(tupload/1024)+" KB/s";
    }
	}
}
