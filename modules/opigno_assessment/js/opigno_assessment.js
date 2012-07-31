
;(function($, window, Drupal, undefined) {
  
  Drupal.behaviors.opignoAssessmentFieldsetSummaries = {
    
    attach: function (context) {
      $('fieldset.opigno-assessment-form', context).drupalSetSummary(function (context) {
        var option = $('input[name="opigno_assessment[option]"]:checked', context).val();
        
        if (option) {
          if (option === 'all') {
            return Drupal.t("All assignments must be completed");
          }
          else if (option === 'some') {
            return Drupal.t("Selected assignments must be completed");
          }
          else {
            return Drupal.t("No assessment");
          }
        }
        else {
          option = $('input[name="opigno_assessment[ponderation]"]', context).val();
          
          return Drupal.t("Ponderation of @num", { '@num': option });
        }
      });
    }
    
  };
  
  Drupal.behaviors.opignoAssessmentForm = {
    
    attach: function(context) {
      var assessmentOption = $('input[name="opigno_assessment[option]"]:checked').val(),
          assessmentRule   = $('input[name="opigno_assessment[rule]"]:checked').val();
      
      // Hide options if needed
      if (assessmentOption !== 'some') {
        $('div.form-item-opigno-assessment-quizzes').hide();
      }
      
      if (assessmentOption === 'no') {
        $('div.form-item-opigno-assessment-rule, div.form-item-opigno-assessment-average, div.form-item-opigno-assessment-min').hide();           
      }
      else if (assessmentRule !== 'average_neg') {
        $('div.form-item-opigno-assessment-min').hide();
      }
      
      $('input[name="opigno_assessment[option]"]').change(function() {
        var $this = $(this);
        
        if ($this.is(':checked')) {
          var  value = $this.val();
          
          if (value === 'some') {
            $('div.form-item-opigno-assessment-quizzes').show();
          }
          else {
            $('div.form-item-opigno-assessment-quizzes').hide();
          }
          
          if (value === 'no') {
            $('div.form-item-opigno-assessment-rule, div.form-item-opigno-assessment-average').hide();
          }
          else {
            $('div.form-item-opigno-assessment-rule, div.form-item-opigno-assessment-average').show();
          }
          
          $this.closest('fieldset.vertical-tabs-pane').trigger('summaryUpdated');
          $this.trigger('formUpdated');
        }
      });
      
      $('input[name="opigno_assessment[rule]"]').change(function() {
        var $this = $(this);
        
        if ($this.is(':checked')) {
          var value = $this.val();
              
          if (value === 'average') {
            $('div.form-item-opigno-assessment-min').hide();
          }
          else {
            $('div.form-item-opigno-assessment-min').show();
          }
          
          $this.trigger('formUpdated');
        }
      });
    }
    
  };
  
})(jQuery, window, Drupal);