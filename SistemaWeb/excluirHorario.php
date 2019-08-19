<?php
 include_once 'conexao.php';
    try{
      $sql = "delete from horarios WHERE id=?";
      $stmt= $PDO->prepare($sql);
      $stmt->execute([$_GET['id']]);
      header('location: index.php');
   }catch(PDOException $erro){
      echo $erro;
   }

?>