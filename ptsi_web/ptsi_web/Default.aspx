<%@ Page Title="PTSI" Language="C#"  AutoEventWireup="true" MasterPageFile="~/Site.master"
    CodeBehind="Default.aspx.cs" Inherits="ptsi_web._Default" %>

<asp:Content ID="MainContentData" runat="server" ContentPlaceHolderID="MainContent">
    <form id="form1" runat="server">
    <div class="SmallMain"> <div class="SmallMainInner">
        <div class="SmallHeader"> PTSI:</div>
        <div>
              <a href="loginpage.aspx"> Logowanie</a>
              <br />
              <a href="patientview.aspx"> Monitoring pacjentów</a>
              <br />
              <a href="patienthistory.aspx"> Dane archiwalne pacjentów</a>
        </div>
    </div> </div>
    </form>
    <asp:Label ID="result" runat="server"></asp:Label>
</asp:Content>
