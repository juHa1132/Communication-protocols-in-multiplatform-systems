<?php

header("Access-Control-Allow-Origin: *");
header("Access-Control-Allow-Headers: *");
header("Content-Type:application/json");

$data = json_decode(file_get_contents('php://input'), true);
$name = $data['name'];
$email = $data['email'];
$text = $data['text'];

use PHPMailer\PHPMailer\PHPMailer;
use PHPMailer\PHPMailer\Exception;
use PHPMailer\PHPMailer\SMTP;

require 'PHPMailer/src/Exception.php';
require 'PHPMailer/src/PHPMailer.php';
require 'PHPMailer/src/SMTP.php';

		try {
			$mail = new PHPMailer();

			$mail->isSMTP();
			$mail->SMTPDebug = SMTP::DEBUG_SERVER;

			$mail->Host = 'smtp.gmail.com';
			$mail->Port = 465;
			$mail->SMTPSecure = PHPMailer::ENCRYPTION_SMTPS;
			$mail->SMTPAuth = true;

			$mail->Username = 'antek.dziurdzia@gmail.com'; // Podaj swój login gmail
			$mail->Password = 'fsekvyxnwafyukke'; // Podaj swoje hasło do aplikacji

			$mail->CharSet = 'UTF-8';
			$mail->setFrom('no-reply@domena.pl', 'Praca inżynierska Antoni Dziurdzia');
			$mail->addAddress('phpodc6@gmail.com');
			$mail->addReplyTo('no-reply@domena.pl', '');

			$mail->isHTML(true);
			$mail->Subject = 'Wiadomość z RaspberryPi';
			$mail->Body = '<html>
	        <head>
	          <title>Nowa wiadomość</title>
	        </head>
	        <body>
	          <h1>Wiadomość od:'. $name .'</h1>
	          <p>'. $text .'
	          </p>
		<p>Podany adres email: '. $email .'
		</p>
	          <hr>
	         
	        </body>
	        </html>
	    	';

	   
	    	$mail->send();


		} catch(Exception $e) {
			echo "Błąd wysyłania maila: {$mail->ErrorInfo}";
		}

?>
<!DOCTYPE html>
<html lang="pl">
<head>

    <meta charset="utf-8">
    <title>Wyślij wiadomość email</title>
    <meta name="description" content="Wysyłanie maili w PHP - funkcja mail(), PHPMailer, SwiftMailer">
    <meta name="keywords" content="php, kurs, PDO, połączenie, MySQL">

    <meta http-equiv="X-Ua-Compatible" content="IE=edge">

    <link rel="stylesheet" href="main.css">
    <link href="https://fonts.googleapis.com/css?family=Lobster|Open+Sans:400,700&amp;subset=latin-ext" rel="stylesheet">
    <!--[if lt IE 9]>
    <script src="https://cdnjs.cloudflare.com/ajax/libs/html5shiv/3.7.3/html5shiv.min.js"></script>
    <![endif]-->

</head>

<body>

    <div class="container">

        <header>
            <h1>Hurra! Wysłaliśmy Ci ebooka!</h1>
        </header>

        <main>
            <article>
                <p class="content">Dziękujemy za zapisanie się na listę mailową naszego newslettera! Link do obiecanego, darmowego ebooka znajdziesz w przysłanej przed chwilą wiadomości! W razie problemów z odnalezieniem maila sprawdź koniecznie zawartość folderu "Spam" w swojej skrzynce pocztowej. Owocnej lektury!</p>
            </article>
        </main>

    </div>

</body>
</html>
