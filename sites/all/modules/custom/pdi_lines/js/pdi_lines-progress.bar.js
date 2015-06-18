(function ($) {
  Drupal.behaviors.pdiProgressChart = {
    attach: function (context, settings) {
      $('.bar_container', context).once('ProgressChart', function () {
        $(document).ready(function() {
          var progressbar = $('#progress_bar');
          max = progressbar.attr('max');
          time = (1000 / max) * 5;
          if ( max == 0) {
            time = 1;
          }
          value = progressbar.val();

          if (max < 25){
            $('.ppc-percents, .ppc-progress').addClass('red');
          } else if (max > 25 && max < 75) {
            $('.ppc-percents, .ppc-progress').addClass('orange');
          } else if (max > 75) {
            $('.ppc-percents, .ppc-progress').addClass('green');
          }

          var loading = function() {
            if (value == max) {
              clearInterval(animate);
            }

            if (max > 0) {
              value += 1;
            };
            
            addValue = progressbar.val(value - 1);

            $('.progress-value').html(value + '%');
            var $ppc = $('.progress-pie-chart'),
            deg = 360 * value / 100;
            if (value > 50) {
              $ppc.addClass('gt-50');
            }
            

            $('.ppc-progress-fill').css('transform', 'rotate(' + deg + 'deg)');
            if (value == 0){
              $('.ppc-percents span').html(value + '%');
            } else {
              $('.ppc-percents span').html(value - 1 + '%');
            }

            if (value == max) {
              clearInterval(animate);
            }
          };

          var animate = setInterval(function() {
            loading();
          }, time);
        });
      });
    }
  };
})(jQuery);