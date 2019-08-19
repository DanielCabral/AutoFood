<?php
    include_once 'conexao.php';
?>
<!DOCTYPE html>
<html lang="en">
<head>
<meta charset="utf-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge,chrome=1">
    <title>AutoFood</title>
    <meta name="description" content="curso de bootstrap 3">
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <link rel="stylesheet" href="https://stackpath.bootstrapcdn.com/bootstrap/4.3.1/css/bootstrap.min.css" integrity="sha384-ggOyR0iXCbMQv3Xipma34MD+dH/1fQ784/j6cY/iJTQUOhcWr7x9JvoRxT2MZw1T" crossorigin="anonymous">
    <script src="jquery-3.4.1.js"></script>
    <script src="js/bootstrap.min.js"></script>
</head>
<body>
<br><br>

<div class="container">
<div class="row col-md-3"></div>
    <div class="row col-md-6 col-md-offset-2 custyle">
    <table class="table table-striped custab">
    <thead>
    <?php
        $consulta = $PDO->query("SELECT horario FROM horarios");
        $count = $consulta->rowCount();
        if($count<32){
    ?>
    <a href="cadastroHorario.html" class="btn btn-primary btn-xs pull-right"><b>+</b> Adicionar novo Horario</a>
    <?php
    }else{
    ?>
    <a href="cadastroHorario.html" class="btn disabled btn-primary btn-xs pull-right"><b>+</b> Adicionar novo Horario</a>
    <?php
    }
    ?>   
    <tr>
            <th>Id de Usuario</th>
            <th>Horario</th>
            <th class="text-center">Opções</th>
        </tr>
    </thead>
            <?php
          $consulta = $PDO->query("SELECT horario,id FROM horarios order by segundos asc;;");
       
          $horarios="";
          while ($linha = $consulta->fetch(PDO::FETCH_ASSOC)) {
            echo "<tr>
            <td>1</td>
            <td>".$linha['horario']."</td>
            <td class=\"text-center\">
            <a href=\"excluirHorario.php?id=".$linha['id']."\" class=\"btn btn-danger btn-xs\"><span class=\"glyphicon glyphicon-remove\"></span> Excluir</a></td>
            </tr>";
              $horarios.=$linha['horario']."@";
          }
           ?>
    </table>
    </div>
    <div class="row col-md-3"></div>
</div>
</body>
</html>