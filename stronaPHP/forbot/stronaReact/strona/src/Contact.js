import React, { useState, useRef } from "react";
import styles from "./Contact.module.css";
import ReCAPTCHA from "react-google-recaptcha";

const Contact = () => {


  const [enteredValues, setEnteredValues] = useState({
    name: "",
    email: "",
    text: "",
  });

  const [didEdit, setDidEdit] = useState({
    name: false,
    email: false,
    text: false,
  });

  const [didStart, setDidStart] = useState({
    email: false,
    name: false,
  });

  const [isFetching, setIsFetching] = useState(false);
  const [isEmailSend, setIsEmailSend] = useState(false);
  const [isError, setIsError] = useState(false);
  // const [consent, setConsent] = useState(false);
  const [captchaValue, setCaptchaValue] = useState(null);
  // const [captchaInvalid, setCaptchaInvalid] = useState(false);
  // const [consentNotChecked, setConsentNotChecked] = useState(false);



  const emailRef = useRef(null);
  // const checkboxRef = useRef(null);

  const emailIsInvalid =
    didStart.email &&
    didEdit.email &&
    !enteredValues.email.includes("@") && 
    !isEmailSend;

  const nameIsInvalid =
    didStart.name && didEdit.name && enteredValues.name === "" && !isEmailSend;


  const handleCaptchaCheck = async () => {
    console.log('captcha check');
    // Wysłanie formularza i tokena reCAPTCHA na serwer
    try {
      const response = await fetch(
        "http://192.168.100.64/forbot/verify_recaptcha.php",
        {
          method: "POST",
          headers: {
            "Content-Type": "application/x-www-form-urlencoded",
          },
          body: `captchaValue=${captchaValue}`,
        }
      );

      const result = await response.json();
      return result.success;
      
    } catch (error) {
      console.error('Captcha verification error',error);
      return false;
    }
  };

  const handleClick = async () => {
    setIsError(false);
    setIsFetching(false);
    // Wysyłanie żądania HTTP POST do skryptu PHP
    if (emailIsInvalid || enteredValues.email === '') {
      
        emailRef.current.focus();
        emailRef.current.scrollIntoView({ behavior: 'smooth', block: 'center' });
        setDidStart((prevState) => ({
          ...prevState,
          email: true, // or any other value you want to set
        }));
        setDidEdit((prevState) => ({
          ...prevState,
          email: true, // or any other value you want to set
        }));
      
      return;
    }
    // if (!consent) {
    //   setConsentNotChecked(true);
    //   checkboxRef.current.focus();
    //   checkboxRef.current.scrollIntoView({ behavior: 'smooth', block: 'center' });

    //   return;
    // }
    setIsFetching(true);
    const isCaptchaValid = await handleCaptchaCheck();
    
    if(!isCaptchaValid){
      alert('Proszę zaznaczyć reCAPTCHA.');
      setIsFetching(false);
      setIsError(true);
      return;
    }

    setIsFetching(true);
    setIsError(false);
    setIsEmailSend(false);
    // setConsentNotChecked(false);

    fetch("http://192.168.100.64/forbot/save.php", {
      method: "POST",
      body: JSON.stringify(enteredValues),
      headers: {
        "Content-Type": "application/json",
      },
    })
      .then((response) => {
        setIsFetching(false);

        if (response.ok) {
          // alert('Mail został wysłany!');
          setIsEmailSend(true);
          setIsError(false);
          // setConsent(false);
          setEnteredValues({ name: "", email: "", text: "" });
        } else {
          setIsError(true);
          // setConsent(false);
          setIsFetching(false);
        }
      })
      .catch((error) => {
        console.error("Wystąpił błąd:", error);
        setIsFetching(false);
        setIsError(true);
        // setConsent(false);
      });
  };

  function handlInputChange(identifier, value) {
    setEnteredValues((prevValues) => ({
      ...prevValues,
      [identifier]: value,
    }));
    setDidEdit((prevEdit) => ({
      ...prevEdit,
      [identifier]: false,
    }));
    setDidStart((prevStart) => ({
      ...prevStart,
      [identifier]: true,
    }));
    setIsEmailSend(false);
  }

  function handleInputBlur(identifier) {
    setDidEdit((prevEdit) => ({
      ...prevEdit,
      [identifier]: true,
    }));
  }

  const handleCaptchaChange = (value) => {
    setCaptchaValue(value);
  };



  return (
    <div className={styles["page-container"]}>
      <div className={styles["contact-container"]}>
        <form className={styles.form}>
          <h1>Formularz kontaktowy</h1>
          <div className={styles["form-item"]}>
            <label>Imię</label>
            <input
              name="name"
              type="name"
              value={enteredValues.name}
              onChange={(event) => handlInputChange("name", event.target.value)}
              onBlur={() => handleInputBlur("name")}
            ></input>
            <span className={styles["bottom-border"]}></span>
            <div className={styles["control-error"]}>
              {nameIsInvalid && <p>Wymagane jest wypełnienie tego pola.</p>}
            </div>
          </div>
          <div className={styles["form-item"]}>
            <label htmlFor="email">Email</label>
            <input
              name="email"
              type="email"
              ref={emailRef}
              value={enteredValues.email}
              onChange={(event) =>
                handlInputChange("email", event.target.value)
              }
              onBlur={() => handleInputBlur("email")}
            ></input>{" "}
            <span className={styles["bottom-border"]}></span>
            <div className={styles["control-error"]}>
              {emailIsInvalid && <p>Proszę wpisać właściwy adres email</p>}
            </div>
          </div>
          <div className={styles["form-item"]}>
            <label>Treść wiadomości</label>
            <textarea
              value={enteredValues.text}
              onChange={(event) => handlInputChange("text", event.target.value)}
              onBlur={() => handleInputBlur("text")}
              type="text"
            ></textarea>
            <span className={styles["bottom-border"]}></span>
          </div>
          <div>
           
            
            <div className={styles["control-error"]}>
            </div>
          </div>
          <div className={styles["captcha-container"]}>
            <ReCAPTCHA
              sitekey="6LdV0_EpAAAAAMB4xwIW0gwWZZqZqYzQytUYvtzZ"
              onChange={handleCaptchaChange}
            />
           
          </div>
         
          {/* <Button classes={buttonStyles} text={"Wyślij"} onClick={handleClick}></Button> */}
          <button
            type="button"
            onClick={handleClick}
            className={styles["button"]}
          
          >
            Wyślij
            <p className={styles["button-icon"]}>❯</p>
          </button>
          {isFetching && <p>Wysyłam maila, proszę czekać...</p>}
          {isEmailSend && (
            <p style={{ fontWeight: "700", color: "#48be48" }}>
              Email został wysłany.
            </p>
          )}
          {isError && (
            <p style={{ fontWeight: "700", color: "red" }}>
              Nie udało się wysłać wiadomości, prosimy spróbować jeszcze raz
              później.
            </p>
          )}
        </form>

   
       
      </div>
    </div>
  );
};

export default Contact;
