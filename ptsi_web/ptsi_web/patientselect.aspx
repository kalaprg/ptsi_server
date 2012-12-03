<%@ Page Title="Wybór pacjentów do monitorowania" Language="C#" AutoEventWireup="true" CodeBehind="patientselect.aspx.cs" 
    MasterPageFile="~/Site.master" Inherits="ptsi_web.patientselect" %>

<asp:Content ID="MainContentData" runat="server" ContentPlaceHolderID="MainContent">
    <div class="Main">
    <div class=SmallMain><div class=SmallMainInner>
    <div class=SmallHeader> Lista pacjentów: </div>
    <div style="padding: 10px">
    <asp:Repeater ID="PatientSelectRepeater" runat="server">
        <HeaderTemplate>
            <table align="center">
                <tr class="Header">
                    <td>Imię</td><td>Nazwisko</td><td>PESEL</td><td>Obserwacja</td>
                </tr>
        </HeaderTemplate>
        <ItemTemplate>
                <tr>
                    <td><%# Eval("firstName") %></td><td><%#Eval("lastName")%></td><td><%#Eval("PESEL")%></td><td><asp:CheckBox runat="server" ID="PScheckbox" Checked=<%#Eval("observation") %> /></td>
                </tr>
        </ItemTemplate>
        <FooterTemplate>
            </table>
        </FooterTemplate>
    </asp:Repeater>
    <br />
    <asp:Button ID="ApplyButton" runat="server" CommandName="Apply" Text="Zatwierdź zmiany" PostBackUrl="~/patientselect.aspx" />
    </div>
    </div> </div>
    </div>
</asp:Content>
