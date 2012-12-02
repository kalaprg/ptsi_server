<%@ Page Title="Logowanie" Language="C#" MasterPageFile="~/Site.master"
    AutoEventWireup="true" CodeBehind="loginpage.aspx.cs" Inherits="ptsi_web.loginpage" %>

<asp:Content ID="MainContentData" runat="server" ContentPlaceHolderID="MainContent">
<form id="form1" runat="server">
    <div class="SmallMain"> <div class="SmallMainInner">
        <div class="SmallHeader"> Logowanie:</div>
        <div>
                
                <div class="FormElement">
                <span class="FormLabel">
                <asp:Label ID="UserNameLabel" runat="server" AssociatedControlID="UserName">Użytkownik:</asp:Label>
                </span>
                <span class="FormValue">
                <asp:TextBox ID="UserName" runat="server"></asp:TextBox>
                </span>
                </div>

                <div class="FormElement">
                <span class="FormLabel">
                <asp:Label ID="PasswordLabel" runat="server" AssociatedControlID="Password">Hasło:</asp:Label>
                </span>
                <span class="FormValue">
                <asp:TextBox ID="Password" runat="server"  TextMode="Password"></asp:TextBox>
                </span>
                </div>

                <div class="FormElement">
                <asp:Button ID="LoginButton" runat="server" CommandName="Login" Text="Zaloguj" ValidationGroup="LoginValidation" PostBackUrl="~/loginpage.aspx" />
                </div>
       
           <br />
            <asp:Label CssClass="FormSubmit" ID="testLabel" runat="server" ></asp:Label>
            <br />
            <asp:Label ID="result" runat="server"> </asp:Label>
        </div>
    </div> </div>
</form>
</asp:Content>
