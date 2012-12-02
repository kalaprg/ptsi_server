<%@ Page Title="Monitorowanie pacjentów" Language="C#" AutoEventWireup="true"
    MasterPageFile="~/Site.master" CodeBehind="patientview.aspx.cs" Inherits="ptsi_web.patientview" %>

<asp:Content ID="MainContentData" runat="server" ContentPlaceHolderID="MainContent">
    
    <div class="Main">
        <asp:Label id="streamLabel" runat="server"></asp:Label>

        <asp:Repeater ID="iframeRepeat" runat="server">
        <ItemTemplate>
            <iframe src="patientframe.aspx?id=<%#Container.ItemIndex%>" width="<%=w%>" height="<%=h%>"></iframe>
        </ItemTemplate>
        </asp:Repeater>

        
    </div>
</asp:Content>
