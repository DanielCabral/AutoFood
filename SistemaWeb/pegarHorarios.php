<?php
    $requisicao=$_GET['requisicao'];
    include_once 'conexao.php';
    $consulta = $PDO->query("SELECT segundos FROM horarios;");
 
    $horarios="";
    while ($linha = $consulta->fetch(PDO::FETCH_ASSOC)) {
        $horarios.=$linha['segundos']."@";
    }
    $horarios=substr ($horarios , 0, strlen ($horarios)-1);
    if($requisicao==10){
        echo "".$horarios;
        //fazer select e enviar
    }else{
        echo "Falhou";
    }
?>