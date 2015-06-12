<div class="dashboard_wrapper">
  <div class="dashboard cards">
    <?php 
      if (isset($progress)) {
        print('<div class="card odd first">'. $progress .'</div>');
      }
    ?>
    <?php 
      if (isset($actions)) {
        print('<div class="card even">'. $actions .'</div>');
      }
    ?>
    <?php 
      if (isset($users)) {
        print('<div class="card odd">'. $users .'</div>');
      }
    ?>
    <?php 
      if (isset($premises)) {
        print('<div class="card even last">'. $premises .'</div>');
      }
    ?>
  </div>
</div>