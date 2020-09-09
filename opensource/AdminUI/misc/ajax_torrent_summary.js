/*
 * $Id: ajax_torrent_summary.js,v 1.2 2007/06/13 09:05:21 wiley Exp $
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

    this.percentageElement = document.getElementById("percentage");
    this.remainsizeElement = document.getElementById("remainsize");
    this.dlrateElement     = document.getElementById("dlrate");
    this.ulrateElement     = document.getElementById("ulrate");
    this.dltotalElement    = document.getElementById("dltotal");
    this.ultotalElement    = document.getElementById("ultotal");

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
    var ajax = new Ajax.Request("ctcs_inquire.php?query=WSENDSTATUS&id="+this.peer_id, {
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
    var elemsCTSTATUS   = res.responseXML.getElementsByTagName("CTSTATUS");
    var elemsCTDETAIL   = res.responseXML.getElementsByTagName("CTDETAIL");
    var elemsCTBW       = res.responseXML.getElementsByTagName("CTBW");

    var i            = this.Index;
    var n_have       = elemsCTSTATUS[i].getElementsByTagName("n_have").item(0).textContent;
    var n_total      = elemsCTSTATUS[i].getElementsByTagName("n_total").item(0).textContent;
    var n_avail      = elemsCTSTATUS[i].getElementsByTagName("n_avail").item(0).textContent;
    var dl_total     = elemsCTSTATUS[i].getElementsByTagName("dl_total").item(0).textContent;
    var ul_total     = elemsCTSTATUS[i].getElementsByTagName("ul_total").item(0).textContent;
    var dl_rate      = elemsCTBW[i].getElementsByTagName("dl_rate").item(0).textContent;
    var ul_rate      = elemsCTBW[i].getElementsByTagName("ul_rate").item(0).textContent;
    var torrent_size = elemsCTDETAIL[i].getElementsByTagName("torrent_size").item(0).textContent;

    if (n_have==null) {
      n_have = parseInt(elemsCTSTATUS[i].childNodes[2].firstChild.nodeValue,10);
    } else { n_have = parseInt(n_have,10); }

    if (n_total==null) {
      n_total = parseInt(elemsCTSTATUS[i].childNodes[3].firstChild.nodeValue,10);
    } else { n_total = parseInt(n_total,10); }

    if (n_avail==null) {
      n_avail = parseInt(elemsCTSTATUS[i].childNodes[4].firstChild.nodeValue,10);
    } else { n_avail = parseInt(n_avail,10); }

    if (dl_total==null) {
      dl_total = parseInt(elemsCTSTATUS[i].childNodes[7].firstChild.nodeValue,10);
    } else { dl_total = parseInt(dl_total,10); }

    if (ul_total==null) {
      ul_total = parseInt(elemsCTSTATUS[i].childNodes[8].firstChild.nodeValue,10);
    } else { ul_total = parseInt(ul_total,10); }

    if (dl_rate==null) {
      dl_rate = parseInt(elemsCTBW[i].childNodes[0].firstChild.nodeValue,10);
    } else { dl_rate = parseInt(dl_rate,10); }

    if (ul_rate==null) {
      ul_rate = parseInt(elemsCTBW[i].childNodes[1].firstChild.nodeValue,10);
    } else { ul_rate = parseInt(ul_rate,10); }

    if (torrent_size==null) {
      torrent_size = parseInt(elemsCTDETAIL[i].childNodes[0].firstChild.nodeValue,10);
    } else { torrent_size = parseInt(torrent_size,10); }

    this.percentageElement.innerHTML = this.percentage(n_have, n_total);
    this.remainsizeElement.innerHTML = this.fileSize(Math.floor(torrent_size*(1-n_have/n_total)));
    this.dlrateElement.innerHTML     = this.speedRate(dl_rate);
    this.ulrateElement.innerHTML     = this.speedRate(ul_rate);
    this.dltotalElement.innerHTML    = this.fileSize(dl_total);
    this.ultotalElement.innerHTML    = this.fileSize(ul_total);
  }
}
