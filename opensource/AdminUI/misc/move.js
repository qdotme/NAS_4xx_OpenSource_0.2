// PickList II script (aka Menu Swapper)- By Phil Webb (http://www.philwebb.com)
// Visit JavaScript Kit (http://www.javascriptkit.com) for this JavaScript and 100s more
// Please keep this notice intact
// Modified By: Wiley Li & Jason Peng
function move (fbox, tbox, arrayName, forward) {
  var arrFbox = new Array();
  var arrTbox = new Array();
  var arrLookup = new Array();
  var itemList;
  var i;
  for(i=0; i<tbox.options.length; i++) {
      arrLookup[tbox.options[i].text] = tbox.options[i].value;
      arrTbox[i] = tbox.options[i].text;
  }
  var fLength = 0;
  var tLength = arrTbox.length
  for(i=0; i<fbox.options.length; i++) {
      arrLookup[fbox.options[i].text] = fbox.options[i].value;
      if(fbox.options[i].selected && fbox.options[i].value != "") {
           arrTbox[tLength] = fbox.options[i].text;
           tLength++;
      } else {
           arrFbox[fLength] = fbox.options[i].text;
           fLength++;
      }
  }
  arrFbox.sort();
  arrTbox.sort();
  fbox.length = 0;
  tbox.length = 0;
  var c;
  for(c=0; c<arrFbox.length; c++) {
      var no = new Option();
      no.value = arrLookup[arrFbox[c]];
      no.text = arrFbox[c];
      fbox[c] = no;
      if(forward == 0) {
        if(c == 0)
          itemList = no.value;
        else
          itemList = itemList + "," + no.value;
      }
  }

  for(c=0; c<arrTbox.length; c++) {
    var no = new Option();
    no.value = arrLookup[arrTbox[c]];
    no.text = arrTbox[c];
    tbox[c] = no;
    if(forward == 1) {
      if(c == 0)
        itemList = no.value;
      else
        itemList = itemList + "," + no.value;
    }
  }
  if (itemList != undefined) {
    arrayName.value = itemList;
  } else {
    arrayName.value = "";
  }
}
