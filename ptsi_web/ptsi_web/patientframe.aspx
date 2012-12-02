<%@ Page Language="C#" AutoEventWireup="true" CodeBehind="patientframe.aspx.cs" Inherits="ptsi_web.patientframe" %>

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">

<html xmlns="http://www.w3.org/1999/xhtml">
<head runat="server">
    <title></title>
    <link href="~/Styles/style.css" rel="stylesheet" type="text/css" />
    <script src="Scripts/webgl-utils.js" type="text/javascript"></script>
    <script src="Scripts/webgl.js" type="text/javascript"></script>
    <script src="Scripts/getdata.js" type="text/javascript"></script>

    
    <script id="vertexshader" type="x-shader/x-vertex">
    attribute vec2 aVertexPosition;
     
    void main() {
            gl_Position = vec4(aVertexPosition, 0.0, 1.0);
    }

    </script>
    <script id="fragmentshader" type="x-shader/x-fragment">
    #ifdef GL_ES
        precision highp float;
    #endif
                           
    uniform vec4 uColor;
                           
    void main() {
            gl_FragColor = uColor;
    }

    </script>
    


    
</head>
<body class="Frame" onload="InitPeriodicUpdate(33, 2000)">
    <form id="form1" runat="server">
    <div class="FrameMain">
        <div class="NameLabel">
            <asp:HiddenField id="patientID" runat="server" />
            <asp:Label id="patientFirstName" runat="server"></asp:Label>
            <asp:Label id="patientLastName" runat="server"></asp:Label>
            <asp:Label id="patientPESEL" runat="server"></asp:Label>
        </div>
        <div width="260" class="Container">
            <canvas id="canvas1" width="256" height="256"></canvas>
            <div class="ValueViewL" id="ECG"></div>
            <div class="ValueViewM" id="pulse"></div>
            <div class="ValueViewR" id="oxygen"></div>
        </div>
        <div>
            <span id="text"></span> <span id="text1"></span> <span id="result"></span>
        </div>
    </div>
    </form>
</body>
</html>
