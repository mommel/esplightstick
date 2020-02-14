function rerenderScreen() {
  $("#bitmaps").empty();
  getBitmaps();   
}

Dropzone.autoDiscover = false;

var dropzone1 = new Dropzone('.dropzone', {
  url: '/images/',
  previewsContainer: '#upload-widget',
  createImageThumbnails: false,
  clickable: "#upload-widget"
});

dropzone1.on("complete", function(file, message) {
  myVar = setTimeout(myVar='' && rerenderScreen, 3000);
});

var lastjson={};
$( document ).ready(function() {
  rerenderScreen();
});

function getBitmaps() {
  var bmp1 = '<div  class="col-md-4 mb-5"><div class="card h-144"><img class="card-img-top" src="../images/';
  var bmp2 = '" alt=""><div class="card-body"><h4 class="card-title">';
  var bmp3 = '</h4></div><div class="card-footer"><span href="#" id="';
  var bmp4 = '" class="btn btn-primary btn-delete">Delete</span></div></div></div>';
  var request = $.ajax({
    url: "http://localhost:3001/images",
    contentType: "application/json",
    method: "GET",
    data: {},
    dataType: "json",
    success: function(result){
      if ( lastjson != result ) {
        lastjson = result;
        $.each(result.data, function( i, value ) {
          var bitmap = bmp1 + value + bmp2 + value +  bmp3 + value + bmp4;
          $( bitmap ).appendTo("#bitmaps");
          $( ".btn-delete" ).click(function() {
              deleteFile($(this).attr("id"));
          })
        })
      }
    }
  })
}

function deleteFile(filename) {
    var request = $.ajax({
        url: "http://localhost:3001/images",
        contentType: "application/json",
        method: "DELETE",
        data: {filename: filename},
        dataType: "json",
        complete: function(result){
          rerenderScreen();
        }
    });   
};
