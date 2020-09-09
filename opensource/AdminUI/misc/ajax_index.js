/*
 * $Id: ajax_index.js,v 1.2 2007/06/13 09:05:21 wiley Exp $
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
  initialize: function(ElementSize, ElementArray, initProperties){
    this.ElementSize       = ElementSize;
    this.ElementArray      = ElementArray;
    this.percentageElement = new Array();
    this.dlElement         = new Array();
    this.ulElement         = new Array();
    this.udrElement        = new Array();
    this.estimatedElement  = new Array();
    this.healthElement     = new Array();
    this.tdlrateElement    = document.getElementById("tdlrate");
    this.tulrateElement    = document.getElementById("tulrate");

    for (var i = 0; i < this.ElementSize; i++) {
      this.percentageElement[i] = document.getElementById("percentage"+this.ElementArray[i]);
      this.dlElement[i]         = document.getElementById("dl"+this.ElementArray[i]);
      this.ulElement[i]         = document.getElementById("ul"+this.ElementArray[i]);
      this.udrElement[i]        = document.getElementById("udr"+this.ElementArray[i]);
      this.estimatedElement[i]  = document.getElementById("estimated"+this.ElementArray[i]);
      this.healthElement[i]     = document.getElementById("health"+this.ElementArray[i]);
    }

    if( initProperties != null ){
      this.frequency   = initProperties["frequency"];
      this.reload_page = initProperties["reload_page"];
    } else { this.frequency = 20; }

    this.isFinish = false;
    this.id = "AjaxCTCSQuery"+ instanceNo++;
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
    var elemsCTORRENT   = res.responseXML.getElementsByTagName("CTORRENT");
    var elemsCTSTATUS   = res.responseXML.getElementsByTagName("CTSTATUS");
    var elemsCTDETAIL   = res.responseXML.getElementsByTagName("CTDETAIL");
    var elemsCTBW       = res.responseXML.getElementsByTagName("CTBW");
    var elems_curtime   = res.responseXML.getElementsByTagName("LastModified");
    var elems_aq        = res.responseXML.getElementsByTagName("aq");
    var elems_aqbool    = res.responseXML.getElementsByTagName("aqbool");
    var elems_qst       = res.responseXML.getElementsByTagName("qst");
    var n_elemsCTSTATUS = elemsCTSTATUS.length;
    var n_elemsCTDETAIL = elemsCTDETAIL.length;
    var n_elemsCTBW     = elemsCTBW.length;

    if (this.ElementSize != n_elemsCTSTATUS || n_elemsCTSTATUS != n_elemsCTDETAIL) {
      setTimeout("window.location='"+this.reload_page+"'", 30000); // refresh page after 30 seconds
    }

    var _v = null;
    var tupload = 0, tdownload = 0;
    for (var i = 0; i < this.ElementSize; i++) {
      var start_timestamp = elemsCTORRENT[this.ElementArray[i]].getElementsByTagName("start_timestamp").item(0).textContent;
      var n_have          = elemsCTSTATUS[this.ElementArray[i]].getElementsByTagName("n_have").item(0).textContent;
      var n_total         = elemsCTSTATUS[this.ElementArray[i]].getElementsByTagName("n_total").item(0).textContent;
      var n_avail         = elemsCTSTATUS[this.ElementArray[i]].getElementsByTagName("n_avail").item(0).textContent;
      var dl_total        = elemsCTSTATUS[this.ElementArray[i]].getElementsByTagName("dl_total").item(0).textContent;
      var ul_total        = elemsCTSTATUS[this.ElementArray[i]].getElementsByTagName("ul_total").item(0).textContent;
      var dl_rate         = elemsCTBW[this.ElementArray[i]].getElementsByTagName("dl_rate").item(0).textContent;
      var ul_rate         = elemsCTBW[this.ElementArray[i]].getElementsByTagName("ul_rate").item(0).textContent;
      var torrent_size    = elemsCTDETAIL[this.ElementArray[i]].getElementsByTagName("torrent_size").item(0).textContent;
      var cur_timestamp   = elems_curtime[0].textContent;
      var aq              = elems_aq[0].textContent;
      var aqbool          = elems_aqbool[0].textContent;
      var qst             = elems_qst[0].textContent;

      if (start_timestamp==null) {
        start_timestamp = parseInt(elemsCTORRENT[this.ElementArray[i]].childNodes[1].firstChild.nodeValue,10);
      } else { start_timestamp = parseInt(start_timestamp,10); }

      if (n_have==null) {
        n_have = parseInt(elemsCTSTATUS[this.ElementArray[i]].childNodes[2].firstChild.nodeValue,10);
      } else { n_have = parseInt(n_have,10); }

      if (n_total==null) {
        n_total = parseInt(elemsCTSTATUS[this.ElementArray[i]].childNodes[3].firstChild.nodeValue,10);
      } else { n_total = parseInt(n_total,10); }

      if (n_avail==null) {
        n_avail = parseInt(elemsCTSTATUS[this.ElementArray[i]].childNodes[4].firstChild.nodeValue,10);
      } else { n_avail = parseInt(n_avail,10); }

      if (dl_total==null) {
        dl_total = parseInt(elemsCTSTATUS[this.ElementArray[i]].childNodes[7].firstChild.nodeValue,10);
      } else { dl_total = parseInt(dl_total,10); }

      if (ul_total==null) {
        ul_total = parseInt(elemsCTSTATUS[this.ElementArray[i]].childNodes[8].firstChild.nodeValue,10);
      } else { ul_total = parseInt(ul_total,10); }

      if (dl_rate==null) {
        dl_rate = parseInt(elemsCTBW[this.ElementArray[i]].childNodes[0].firstChild.nodeValue,10);
      } else { dl_rate = parseInt(dl_rate,10); }

      if (ul_rate==null) {
        ul_rate = parseInt(elemsCTBW[this.ElementArray[i]].childNodes[1].firstChild.nodeValue,10);
      } else { ul_rate = parseInt(ul_rate,10); }

      if (torrent_size==null) {
        torrent_size = parseInt(elemsCTDETAIL[this.ElementArray[i]].childNodes[0].firstChild.nodeValue,10);
      } else { torrent_size = parseInt(torrent_size,10); }

      if (cur_timestamp==null) {
        cur_timestamp = parseInt(elems_curtime[0].firstChild.nodeValue,10);
      } else { cur_timestamp = parseInt(cur_timestamp,10); }

      if (aq==null) {
        aq = parseInt(elems_aq[0].firstChild.nodeValue,10);
      } else { aq = parseInt(aq,10); }

      if (aqbool==null) {
        aqbool = parseInt(elems_aqbool[0].firstChild.nodeValue,10);
      } else { aqbool = parseInt(aqbool,10); }

      if (qst==null) {
        qst = parseInt(elems_qst[0].firstChild.nodeValue,10);
      } else { qst = parseInt(qst,10); }

      var cal_qst = false;
      if (dl_rate==0 && ul_rate==0) {
        document.images["simg"+this.ElementArray[i]].src=img_connect_p.src;
      } else if (ul_rate>0 && n_have>=n_total) {
        document.images["simg"+this.ElementArray[i]].src=img_seed.src;
      } else if (dl_rate>0 && n_have<n_total) {
        document.images["simg"+this.ElementArray[i]].src=img_peer.src;
      }

      if (n_have>=n_total) {
        if (aq==1&&aqbool==0) { // enable auto-stop-bt-task and use OR condition
          cal_qst = true;
        }
      }

      this.percentageElement[this.ElementArray[i]].innerHTML = this.percentage(n_have, n_total);
      this.dlElement[this.ElementArray[i]].innerHTML         = this.speedRate(dl_rate);
      this.ulElement[this.ElementArray[i]].innerHTML         = this.speedRate(ul_rate);
      this.udrElement[this.ElementArray[i]].innerHTML        = this.udRatio(ul_total, dl_total);
      if (!cal_qst) {
        this.estimatedElement[this.ElementArray[i]].innerHTML  = this.estimatedTime(Math.floor((torrent_size*(1-n_have/n_total))/dl_rate));
      } else {
        this.estimatedElement[this.ElementArray[i]].innerHTML  = this.estimatedTime(qst*60-(cur_timestamp-start_timestamp));
      }

      this.healthElement[i].innerHTML     = this.percentage(n_avail, n_total);

      tdownload += dl_rate;
      tupload   += ul_rate;
    }

    this.tdlrateElement.innerHTML = this.speedRate(tdownload);
    this.tulrateElement.innerHTML = this.speedRate(tupload);
  }
}
