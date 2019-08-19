<?php
   include_once 'conexao.php';
   if($_SERVER["REQUEST_METHOD"] == "POST") {
      // username and password sent from form 
      $hora = $_POST['horario'];
      $formattedtime = date('h:i', strtotime($_POST['horario']));
      //echo $formattedtime;
      $dt = DateTime::createFromFormat('H:i', $hora); // create today 10 o'clock 
      $hora=$dt->format('H');       
      $minutos=$dt->format('i');          
      $segundos=($hora*3600)+($minutos*60);
      $horario=$dt->format('H:i');     
      echo "<br>";
      echo $segundos;
      try{
         $sql = "INSERT INTO horarios (horario,segundos) VALUES (?,?)";
         $stmt= $PDO->prepare($sql);
         $stmt->execute([$horario,$segundos]);
         header('location: index.php');
      }catch(PDOException $erro){
         echo $erro;
      }
   }
?>