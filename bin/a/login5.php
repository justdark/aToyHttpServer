
<!DOCTYPE html>

<html>
  <head>
    <title>Log In</title>
	<style type="text/css">
		.all{
		width:350px; 
		background:#abc; 
		z-index:99; 
		overflow:auto;
		padding:20px
		}
</style>
  </head>
  <body>
  <div class='all'>
    <form action="/dynamic_src/src4/login/login6.php" method="post">
      <table>
        <tr>
          <td>Username:</td>
          <td>
            <input name="user" type="text"></td>
        </tr>
        <tr>
          <td>Password:</td>
          <td><input name="pass" type="password"></td>
        </tr>
        <tr>
          <td></td>
          <td><input type="submit" value="Log In"></td>
        </tr>
      </table>      
    </form>
	</div>
  </body>
</html>
