<?php

header("Access-Control-Allow-Origin: *");
header("Access-Control-Allow-Headers: *");
header("Content-Type:application/json");

if ($_SERVER['REQUEST_METHOD'] === 'POST') {
    $recaptchaSecret = '6LdV0_EpAAAAAHIkS7g9IQntlhHjSdssDz6pabXp';
    $recaptchaResponse = $_POST['captchaValue'];

    // Weryfikacja, czy token został dostarczony
    if (empty($recaptchaResponse)) {
        echo json_encode(['success' => false, 'message' => 'Token reCAPTCHA jest wymagany.']);
        exit;
    }

    // Weryfikacja tokena reCAPTCHA z serwerem Google
    $verifyUrl = 'https://www.google.com/recaptcha/api/siteverify';
    $response = file_get_contents($verifyUrl . '?secret=' . $recaptchaSecret . '&response=' . $recaptchaResponse);
    $responseKeys = json_decode($response, true); 

    if ($responseKeys["success"]) {
        // Weryfikacja reCAPTCHA powiodła się
        echo json_encode(['success' => true]);
    } else {
        // Weryfikacja reCAPTCHA nie powiodła się
        echo json_encode(['success' => false, 'message' => 'Weryfikacja reCAPTCHA nie powiodła się.']);
    }
} else {
    echo json_encode(['success' => false, 'message' => 'Niewłaściwa metoda żądania.']);
}

?>
