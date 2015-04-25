$(function(){
  $(document).load(function(){
    updateSize();
  });

  $("#fileElem").change(function(){
    updateSize();
    handleFiles();
  });

  // $(".sendbtn").click(function(){
  //   // alert("ssd");
  // });

  var fileSelect = $("#fileSelect"),
      fileElem = $("#fileElem");
  fileSelect.on("click",function(e){
    if (fileElem) {
      fileElem.click();
    }
    e.preventDefault();
  })

})

function updateSize() {
  var nBytes = 0,
      oFiles = $("#fileElem")[0].files,
      nFiles = oFiles.length;
      console.log("file is"+oFiles);
  for (var nFileId = 0; nFileId < nFiles; nFileId++) {
    nBytes += oFiles[nFileId].size;
  }
  var sOutput = nBytes + " bytes";
  // optional code for multiples approximation
  for (var aMultiples = ["KiB", "MiB", "GiB", "TiB", "PiB", "EiB", "ZiB", "YiB"], nMultiple = 0, nApprox = nBytes / 1024; nApprox > 1; nApprox /= 1024, nMultiple++) {
    sOutput = nApprox.toFixed(3) + " " + aMultiples[nMultiple] + " (" + nBytes + " bytes)";
  }
  // end of optional code
  document.getElementById("fileNum").innerHTML = nFiles;
  document.getElementById("fileSize").innerHTML = sOutput;
}

function sendFile() {
  var oFiles = $("#uploadInput")[0].files;
  console.log(oFiles.id);
}

function handleFiles() {
  var files = $("#fileElem")[0].files;
  for (var i = 0; i < files.length; i++ ){
    var file = files[i];
    var imageType = /image.*/;

    if (!file.type.match(imageType)) {
      continue;
    }

    var img = $("<img>");
    img.addClass("obj");
    img.file = file;
    img.appendTo($("body"));

    var reader = new FileReader();
    reader.onload = function (e){
      img.attr("src",e.target.result)
    }

    reader.readAsDataURL(file);

  }
}