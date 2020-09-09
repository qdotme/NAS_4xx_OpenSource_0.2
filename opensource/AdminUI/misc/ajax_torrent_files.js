/*
 * $Id: ajax_torrent_files.js,v 1.2 2007/06/13 09:05:21 wiley Exp $
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
var AjaxCTCSQuery = Class.create();
AjaxCTCSQuery.prototype = {
  initialize: function(Index, initProperties){
    this.Index = Index;

    if( initProperties != null ){
      this.frequency   = initProperties["frequency"];
      this.reload_page = initProperties["reload_page"];
      this.peer_id     = initProperties["peer_id"];
    } else { this.frequency = 20; }

    this.isFinish = false;
    this.id = "AjaxCTCSQuery"+ instanceNo++;
    window[this.id] = this;
  },
  start: function(){ this.update(); },
  update: function(){
    var upThis = this;
    var ajax = new Ajax.Request("ctcs_inquire.php?query=WSENDDETAIL&id="+this.peer_id, {
      method: "get",
      onSuccess: function(res){ upThis.updateSuccess(res); },
      onFailure: function(res){ alert("Error: "+ res.status +" "+ res.statusText);}
    });
    if( !this.isFinish ){
      setTimeout("window."+ this.id +".update()", this.frequency*1000);
    }
  },
  fileSize: function(size){
    if(size <= 0) { return "0 KB"; }

    if(size >= 1073741824) {
      size = Math.round(size / 1073741824 * 100) / 100 + " GB";
    } else if(size >= 1048576) {
      size = Math.round(size / 1048576 * 100) / 100 + " MB";
    } else {
      size = Math.round(size / 1024 * 100) / 100 + " KB";
    }

    return size;
  },
  estimatedTime: function(time){
    if(time > 0) {
      var hours   = Math.floor(time / 3600);
      var minutes = Math.floor((time - (hours * 3600)) / 60);
      var seconds = Math.floor(time - (hours * 3600) - (minutes * 60));

      if(hours <= 99) {
        return ((hours<=9)?"0"+hours:hours) +
               ":" +
               ((minutes<=9)?"0"+minutes:minutes) +
               ":" +
               ((seconds<=9)?"0"+seconds:seconds);
      }
    }

    return "99:99:99";
  },
  speedRate: function(rate){
    if(rate <= 0) { return "0 KB/s"; }

    if(rate >= 1048576) {
      rate = Math.round(rate / 1048576 * 100) / 100 + " MB/s";
    } else {
      rate = Math.round(rate / 1024 * 100) / 100 + " KB/s";
    }

    return rate;
  },
  percentage: function(have, total){
    if(total <= 0) { return "0.0%"; }

    var percent = Math.round(100 * have / total * 10) / 10;

    if(percent >= 100) { return "100%"; }

    return percent.toFixed(1) + "%";
  },
  udRatio: function(ul, dl){
    if(dl < 1){
      dl = 1;
    }

    var udr = ul / dl;
    if(udr >= 10) {
      return "&gt; 10";
    }

    return udr.toFixed(1);
  },
  updateSuccess: function(res){
    var elemsCTFILE   = res.responseXML.getElementsByTagName("CTFILE")[this.Index];
    var n_elemsCTFILE = elemsCTFILE.getElementsByTagName("filename").length;

    for(var i = 1; i <= n_elemsCTFILE; i++) {
      var n_pieces = elemsCTFILE.getElementsByTagName("F"+i)[0].getElementsByTagName("n_pieces").item(0).textContent;
      var n_have   = elemsCTFILE.getElementsByTagName("F"+i)[0].getElementsByTagName("n_have").item(0).textContent;
      var filesize = elemsCTFILE.getElementsByTagName("F"+i)[0].getElementsByTagName("filesize").item(0).textContent;
      var filename = elemsCTFILE.getElementsByTagName("F"+i)[0].getElementsByTagName("filename").item(0).textContent;

      if (n_pieces==null) {
        n_pieces = parseInt(elemsCTFILE.getElementsByTagName("F"+i)[0].childNodes[0].firstChild.nodeValue,10);
      } else { n_pieces = parseInt(n_pieces,10); }

      if (n_have==null) {
        n_have = parseInt(elemsCTFILE.getElementsByTagName("F"+i)[0].childNodes[1].firstChild.nodeValue,10);
      } else { n_have = parseInt(n_have,10); }

      if (filesize==null) {
        filesize = parseInt(elemsCTFILE.getElementsByTagName("F"+i)[0].childNodes[2].firstChild.nodeValue,10);
      } else { filesize = parseInt(filesize,10); }

      if (filename==null) {
        filename = elemsCTFILE.getElementsByTagName("F"+i)[0].childNodes[3].firstChild.nodeValue;
      }

      var percentageElement = document.getElementById("percentage"+i);
      var filenameElement   = document.getElementById("filename"+i);

      if (filenameElement.innerHTML == filename) {
        percentageElement.innerHTML = this.percentage(n_have, n_pieces);
      }
    }
  }
}
