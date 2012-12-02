var dataECG = new Array();
var readIndexECG = 0;
var writeIndexECG = 255;
var CANVAS_WIDTH = 256;
var CANVAS_HEIGHT = 256;

function InitData()
{

    readIndexECG = 0;
    writeIndexECG = 255;
    c1 = 0;
    c2 = 0;
    for(var i=0; i<256; ++i)
    {
        dataECG[i] = 0;
    }
}



function UpdateIndex(index)
{
 
    if (index > 255) {
        return 0;
    }
    else {
        return index + 1;
    }
}

//Create graph from given data
function GenerateImage(data)
{
    //var dataArray = data.split(",");

    //alert(data.toString());

    dataECG[writeIndexECG] = data;

    //document.getElementById("result").innerHTML = dataECG.toString();
    writeIndexECG = UpdateIndex(writeIndexECG);
    readIndexECG = UpdateIndex(readIndexECG);

    //document.getElementById("ECG").innerHTML = readIndexECG + " " + writeIndexECG;

    var width = 256;
    var height = 256;
    var scale = 1;

    //var context = document.getElementById("canvas1").getContext("2d");

    
    //document.getElementById("result").innerHTML = data.toString();

    //context.strokeStyle = 'red';
    

    /*var imgData = context.getImageData(1, 0, width - 1, height);
    context.clearRect(0, 0, width, height);
    context.putImageData(imgData, 0, 0);

    var x = writeIndexECG-1;
    var nextX = UpdateIndex(x);

    var i = 254;
    var bx = i * scale;
    var by = 255 - dataECG[x];
    var ex = (i + 1) * scale;
    var ey = 255 - dataECG[nextX];

    context.beginPath();
    context.moveTo(bx, by);
    context.lineTo(ex, ey);
    context.stroke();*/

    //context.clearRect(0, 0, width*scale, height);
    //Draw();

    /*context.beginPath();
    var x = readIndexECG;
    for (var i = 0; i < width - 2; ++i)
    {

        var nextX = UpdateIndex(x);
        var bx = i * scale;
        var by = 255 - dataECG[x];
        var ex = (i + 1) * scale;
        var ey = 255 - dataECG[nextX];

        
        context.moveTo(bx, by);
        context.lineTo(ex, ey);
        

        x = UpdateIndex(x);
    }
    context.stroke();*/
    document.getElementById("ECG").innerHTML = data;
}

function WritePulseValue(data) {

    //var dataArray = data.split(",");
    document.getElementById("pulse").innerHTML = data;
}

function WriteOxygenValue(data)
{
    //var dataArray = data.split(",");
    document.getElementById("oxygen").innerHTML = data;
}


function Response(xmlhttp, page, datatype) {

    if (xmlhttp.readyState == 4 && xmlhttp.status == 200) {
        
        switch (datatype) {
            case 2:
                GenerateImage(xmlhttp.responseText);
                break;
            case 0:
                WritePulseValue(xmlhttp.responseText);
                break;
            case 1:
                WriteOxygenValue(xmlhttp.responseText);
                break;
            default:
        }
    }
}


//common AJAX connection function
function loadXMLDoc(page, datatype)
{
    var xmlhttp;
    if (window.XMLHttpRequest) xmlhttp = new XMLHttpRequest();
    else xmlhttp = new ActiveXObject("Microsoft.XMLHTTP");

    xmlhttp.onreadystatechange = function () {
        Response(xmlhttp, page, datatype);
    };
    var id = document.getElementById("patientID").getAttribute("value")

    xmlhttp.open("GET", page+ "?id=" + id + "&datatype=" + datatype+ "&t=" + Math.random(), true);
    xmlhttp.send();
}

var c1 = 0;
var c2 = 0;

//updates frame using local data
function UpdateFrame()
{

    //alert("updateframe");
    ++c1;
    document.getElementById("text1").innerHTML = c1.toString(10);

    loadXMLDoc("patientcurrentdata.aspx", 0);
    loadXMLDoc("patientcurrentdata.aspx", 1);
    loadXMLDoc("patientcurrentdata.aspx", 2);
}

//gets more data from previously defined data source
function GetData()
{

    //alert("getdata");
    ++c2;
    document.getElementById("text").innerHTML = c2.toString(10);
    loadXMLDoc("patientframe.aspx", -1);

}

function InitPeriodicUpdate(frameUpdateInterval, getDataInterval)
{

    //alert("initupdate");
    setInterval(UpdateFrame, frameUpdateInterval);
    setInterval(GetData, getDataInterval);
    InitData();
    InitWebGL();

}