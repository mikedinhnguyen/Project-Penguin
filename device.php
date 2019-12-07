<!DOCTYPE html>
    <html>
        <head>
            <title>Project Penguin</title>
            <link rel="stylesheet" type="text/css" href="global.css">
            <link rel="shortcut icon" type="image/x-icon" href="penguin.png" />
        </head>
        <body>
            <div class="form-container">
                <div class="user-img"></div>
                <form action="hub.php" method="POST">
                <ul class="list">
                    <li>Device Check</li>
                    <li><form name="form1" method="post" action="hub.php">
                            <input name="submit" type="submit" value="Device 1">
                        </form>
                    </li>
                    <li><input name="submit2" type="submit" value="Device 2 (coming soon)"></li>
                    <li><input name="submit3" type="submit" value="Device 3 (coming soon)"></li>
                    <li><form name="form1" method="post" action="login.php">
                    <input name="submit2" type="submit" id="submit2" value="Log out">
                    </form>
                    </li>
                </ul>
                </form>
            </div>
        </body>
    </html>
