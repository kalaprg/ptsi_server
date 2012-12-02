<%@ Page Language="C#" AutoEventWireup="true" CodeBehind="patienthistory.aspx.cs" Inherits="ptsi_web.patienthistory" %>

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">

<html xmlns="http://www.w3.org/1999/xhtml">
<head runat="server">
    <title></title>
    <link href="~/Styles/style.css" rel="stylesheet" type="text/css" />
</head>
<body>
    <form id="form1" runat="server">
    <div>
        <asp:Image ID="imageECG" runat="server" />
        <br />
        <asp:Image ID="imagePulse" runat="server" />
        <br />
        <asp:Image ID="imageOxygen" runat="server" />
    </div>
    </form>
</body>
</html>
