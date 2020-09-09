/*
 * $Id: ajax_torrent_peers.js,v 1.2 2007/06/13 09:05:21 wiley Exp $
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
    this.peertblElement = document.getElementById("peertbl");

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
    var ajax = new Ajax.Request("ctcs_inquire.php?query=WSENDPEERS&id="+this.peer_id, {
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
    var elemsCTSTATUS = res.responseXML.getElementsByTagName("CTSTATUS")[this.Index];
    var elemsCTPEER   = res.responseXML.getElementsByTagName("CTPEER")[this.Index];
    var n_elemsCTPEER = elemsCTPEER.getElementsByTagName("peer_id").length;
    var n_total       = elemsCTSTATUS.childNodes[3].firstChild.nodeValue;

    var peers_table = "<table width=\"100%\" border=\"0\" cellspacing=\"0\" cellpadding=\"0\">" +
                      "<tr>" +
                      "<td width=\"15%\" class=\"listhdrr\">"+tl_ip+"</td>" +
                      "<td width=\"1%\" class=\"listhdrr\">%</td>" +
                      "<td width=\"15%\" class=\"listhdrr\" nowrap>"+tl_dn+"</td>" +
                      "<td width=\"15%\" class=\"listhdrr\" nowrap>"+tl_up+"</td>" +
                      "<td width=\"15%\" class=\"listhdrr\">"+tl_tdl+"</td>" +
                      "<td width=\"15%\" class=\"listhdrr\">"+tl_tul+"</td>" +
                      "<td width=\"*\" class=\"listhdrr\" nowrap>"+tl_client+"</td>" +
                      "</tr>";
    for(var i = 1; i <= n_elemsCTPEER; i++) {
      var peer_id        = elemsCTPEER.getElementsByTagName("P"+i)[0].getElementsByTagName("peer_id").item(0).textContent;
      var address        = elemsCTPEER.getElementsByTagName("P"+i)[0].getElementsByTagName("address").item(0).textContent;
      var choke_interest = elemsCTPEER.getElementsByTagName("P"+i)[0].getElementsByTagName("choke_interest").item(0).textContent;
      var dl_rate        = elemsCTPEER.getElementsByTagName("P"+i)[0].getElementsByTagName("dl_rate").item(0).textContent;
      var ul_rate        = elemsCTPEER.getElementsByTagName("P"+i)[0].getElementsByTagName("ul_rate").item(0).textContent;
      var dl_total       = elemsCTPEER.getElementsByTagName("P"+i)[0].getElementsByTagName("dl_total").item(0).textContent;
      var ul_total       = elemsCTPEER.getElementsByTagName("P"+i)[0].getElementsByTagName("ul_total").item(0).textContent;
      var n_pieces       = elemsCTPEER.getElementsByTagName("P"+i)[0].getElementsByTagName("n_pieces").item(0).textContent;
      var client_type    = elemsCTPEER.getElementsByTagName("P"+i)[0].getElementsByTagName("client_type").item(0).textContent;

      if (peer_id==null) {
        peer_id = elemsCTPEER.getElementsByTagName("P"+i)[0].childNodes[0].firstChild.nodeValue;
      }

      if (address==null) {
        address = elemsCTPEER.getElementsByTagName("P"+i)[0].childNodes[1].firstChild.nodeValue;
      }

      if (choke_interest==null) {
        choke_interest = elemsCTPEER.getElementsByTagName("P"+i)[0].childNodes[2].firstChild.nodeValue;
      }

      if (dl_rate==null) {
        dl_rate = parseInt(elemsCTPEER.getElementsByTagName("P"+i)[0].childNodes[3].firstChild.nodeValue,10);
      } else { dl_rate = parseInt(dl_rate,10); }

      if (ul_rate==null) {
        ul_rate = parseInt(elemsCTPEER.getElementsByTagName("P"+i)[0].childNodes[4].firstChild.nodeValue,10);
      } else { ul_rate = parseInt(ul_rate,10); }

      if (dl_total==null) {
        dl_total = parseInt(elemsCTPEER.getElementsByTagName("P"+i)[0].childNodes[5].firstChild.nodeValue,10);
      } else { dl_total = parseInt(dl_total,10); }

      if (ul_total==null) {
        ul_total = parseInt(elemsCTPEER.getElementsByTagName("P"+i)[0].childNodes[6].firstChild.nodeValue,10);
      } else { ul_total = parseInt(ul_total,10); }

      if (n_pieces==null) {
        n_pieces = parseInt(elemsCTPEER.getElementsByTagName("P"+i)[0].childNodes[7].firstChild.nodeValue,10);
      } else { n_pieces = parseInt(n_pieces,10); }

      if (client_type==null) {
        client_type = elemsCTPEER.getElementsByTagName("P"+i)[0].childNodes[8].firstChild.nodeValue;
      }

      /*
      var peer_id        = elemsCTPEER.getElementsByTagName("P"+i)[0].childNodes[0].firstChild.nodeValue;
      var address        = elemsCTPEER.getElementsByTagName("P"+i)[0].childNodes[1].firstChild.nodeValue;
      var choke_interest = elemsCTPEER.getElementsByTagName("P"+i)[0].childNodes[2].firstChild.nodeValue;
      var dl_rate        = elemsCTPEER.getElementsByTagName("P"+i)[0].childNodes[3].firstChild.nodeValue;
      var ul_rate        = elemsCTPEER.getElementsByTagName("P"+i)[0].childNodes[4].firstChild.nodeValue;
      var dl_total       = elemsCTPEER.getElementsByTagName("P"+i)[0].childNodes[5].firstChild.nodeValue;
      var ul_total       = elemsCTPEER.getElementsByTagName("P"+i)[0].childNodes[6].firstChild.nodeValue;
      var n_pieces       = elemsCTPEER.getElementsByTagName("P"+i)[0].childNodes[7].firstChild.nodeValue;
      var client_type    = elemsCTPEER.getElementsByTagName("P"+i)[0].childNodes[8].firstChild.nodeValue;
      */

      peers_table += "<tr>" +
                     "<td class=\"listlr\">"+address+"</td>" +
                     "<td class=\"listr\">"+this.percentage(n_pieces,n_total)+"</td>" +
                     "<td class=\"listr\">"+this.speedRate(dl_rate)+"</td>" +
                     "<td class=\"listr\">"+this.speedRate(ul_rate)+"</td>" +
                     "<td class=\"listr\">"+this.fileSize(dl_total)+"</td>" +
                     "<td class=\"listr\">"+this.fileSize(ul_total)+"</td>" +
                     "<td class=\"listr\" nowrap>"+client_type+"</td>" +
                     "</tr>";
    }
    peers_table += "</table>";

    this.peertblElement.innerHTML = peers_table;
  }
}
