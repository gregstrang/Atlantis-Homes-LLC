// JavaScript Document
function numberFormat(thisNumber) {
  if( thisNumber.length == 0 ) {
	  thisNumber = 0;		
  } else {
	  thisNumber = thisNumber.replace(",", "");
	  thisNumber = parseFloat(thisNumber);
  }
  if(isNaN(thisNumber)){
	  thisNumber = 0;
  }
  return thisNumber;
}

function calculateBudget() {
  var thisForm = $("form[name='worksheet']").attr("id");
  thisForm = document.getElementById(thisForm);
  var thisRunningIncomeTotal = 0.0;
  var thisRunningExpenseTotal = 0.0;
  var tempNum = 0.0;
  
  

 if( thisForm['submitted[lender_investment_formula__budget][gross_monthly_income][weekly_amount][job_1]'].value.length == 0 ) {
	  thisForm['submitted[lender_investment_formula__budget][gross_monthly_income][monthly_amount][job_monthly_amount_1]'].value = '';
  } else {
	  thisForm['submitted[lender_investment_formula__budget][gross_monthly_income][monthly_amount][job_monthly_amount_1]'].value = numberFormat(thisForm['submitted[lender_investment_formula__budget][gross_monthly_income][weekly_amount][job_1]'].value) * numberFormat('4.33');
  }
  tempNum = numberFormat(thisForm['submitted[lender_investment_formula__budget][gross_monthly_income][monthly_amount][job_monthly_amount_1]'].value);
  thisForm['submitted[lender_investment_formula__budget][gross_monthly_income][monthly_amount][job_monthly_amount_1]'].value = tempNum.toFixed(2);
  thisRunningIncomeTotal += numberFormat(thisForm['submitted[lender_investment_formula__budget][gross_monthly_income][monthly_amount][job_monthly_amount_1]'].value);
  
  if( thisForm['submitted[lender_investment_formula__budget][gross_monthly_income][weekly_amount][job_2]'].value.length == 0 ) {
	  thisForm['submitted[lender_investment_formula__budget][gross_monthly_income][monthly_amount][job_monthly_amount_2]'].value = '';
  } else {
	  thisForm['submitted[lender_investment_formula__budget][gross_monthly_income][monthly_amount][job_monthly_amount_2]'].value = numberFormat(thisForm['submitted[lender_investment_formula__budget][gross_monthly_income][weekly_amount][job_2]'].value) * numberFormat('4.33');
  }
  tempNum = numberFormat(thisForm['submitted[lender_investment_formula__budget][gross_monthly_income][monthly_amount][job_monthly_amount_2]'].value);
  thisForm['submitted[lender_investment_formula__budget][gross_monthly_income][monthly_amount][job_monthly_amount_2]'].value = tempNum.toFixed(2);
  thisRunningIncomeTotal += numberFormat(thisForm['submitted[lender_investment_formula__budget][gross_monthly_income][monthly_amount][job_monthly_amount_2]'].value);
  
  if( thisForm['submitted[lender_investment_formula__budget][gross_monthly_income][weekly_amount][job_3]'].value.length == 0 ) {
	  thisForm['submitted[lender_investment_formula__budget][gross_monthly_income][monthly_amount][job_monthly_amount_3]'].value = '';
  } else {
	  thisForm['submitted[lender_investment_formula__budget][gross_monthly_income][monthly_amount][job_monthly_amount_3]'].value = numberFormat(thisForm['submitted[lender_investment_formula__budget][gross_monthly_income][weekly_amount][job_3]'].value) * numberFormat('4.33');
  }
  tempNum = numberFormat(thisForm['submitted[lender_investment_formula__budget][gross_monthly_income][monthly_amount][job_monthly_amount_3]'].value);
  thisForm['submitted[lender_investment_formula__budget][gross_monthly_income][monthly_amount][job_monthly_amount_3]'].value = tempNum.toFixed(2);
  thisRunningIncomeTotal += numberFormat(thisForm['submitted[lender_investment_formula__budget][gross_monthly_income][monthly_amount][job_monthly_amount_3]'].value);
  

  
  if( thisForm['submitted[lender_investment_formula__budget][gross_monthly_income][weekly_amount][other_income_1]'].value.length == 0 ) {
	  thisForm['submitted[lender_investment_formula__budget][gross_monthly_income][monthly_amount][other_monthly_amount_1]'].value = '';
  } else {
	  thisForm['submitted[lender_investment_formula__budget][gross_monthly_income][monthly_amount][other_monthly_amount_1]'].value = numberFormat(thisForm['submitted[lender_investment_formula__budget][gross_monthly_income][weekly_amount][other_income_1]'].value) * 4.33;
  }
  tempNum = numberFormat(thisForm['submitted[lender_investment_formula__budget][gross_monthly_income][monthly_amount][other_monthly_amount_1]'].value);
  thisForm['submitted[lender_investment_formula__budget][gross_monthly_income][monthly_amount][other_monthly_amount_1]'].value = tempNum.toFixed(2);
  thisRunningIncomeTotal += numberFormat(thisForm['submitted[lender_investment_formula__budget][gross_monthly_income][monthly_amount][other_monthly_amount_1]'].value);
  
  if( thisForm['submitted[lender_investment_formula__budget][gross_monthly_income][weekly_amount][other_income_2]'].value.length == 0 ) {
	  thisForm['submitted[lender_investment_formula__budget][gross_monthly_income][monthly_amount][other_monthly_amount_2]'].value = '';
  } else {
	  thisForm['submitted[lender_investment_formula__budget][gross_monthly_income][monthly_amount][other_monthly_amount_2]'].value = numberFormat(thisForm['submitted[lender_investment_formula__budget][gross_monthly_income][weekly_amount][other_income_2]'].value) * 4.33;
  }
  tempNum = numberFormat(thisForm['submitted[lender_investment_formula__budget][gross_monthly_income][monthly_amount][other_monthly_amount_2]'].value);
  thisForm['submitted[lender_investment_formula__budget][gross_monthly_income][monthly_amount][other_monthly_amount_2]'].value = tempNum.toFixed(2);
  thisRunningIncomeTotal += numberFormat(thisForm['submitted[lender_investment_formula__budget][gross_monthly_income][monthly_amount][other_monthly_amount_2]'].value);
  
  if( thisForm['submitted[lender_investment_formula__budget][gross_monthly_income][weekly_amount][other_income_3]'].value.length == 0 ) {
	  thisForm['submitted[lender_investment_formula__budget][gross_monthly_income][monthly_amount][other_monthly_amount_3]'].value = '';
  } else {
	  thisForm['submitted[lender_investment_formula__budget][gross_monthly_income][monthly_amount][other_monthly_amount_3]'].value = numberFormat(thisForm['submitted[lender_investment_formula__budget][gross_monthly_income][weekly_amount][other_income_3]'].value) * 4.33;
  }
  tempNum = numberFormat(thisForm['submitted[lender_investment_formula__budget][gross_monthly_income][monthly_amount][other_monthly_amount_3]'].value);
  thisForm['submitted[lender_investment_formula__budget][gross_monthly_income][monthly_amount][other_monthly_amount_3]'].value = tempNum.toFixed(2);
  thisRunningIncomeTotal += numberFormat(thisForm['submitted[lender_investment_formula__budget][gross_monthly_income][monthly_amount][other_monthly_amount_3]'].value);
  

  thisForm['submitted[lender_investment_formula__budget][total_monthly_gross_income]'].value = thisRunningIncomeTotal;

  
  
	  thisRunningExpenseTotal += numberFormat(thisForm['submitted[lender_investment_formula__budget][monthly_obligations][monthly_amount][car_payment_1]'].value);
  
	  thisRunningExpenseTotal += numberFormat(thisForm['submitted[lender_investment_formula__budget][monthly_obligations][monthly_amount][car_payment_2]'].value);
  
	  thisRunningExpenseTotal += numberFormat(thisForm['submitted[lender_investment_formula__budget][monthly_obligations][monthly_amount][car_payment_3]'].value);
  
  
	  thisRunningExpenseTotal += numberFormat(thisForm['submitted[lender_investment_formula__budget][monthly_obligations][monthly_amount][installment_loan_1]'].value);
  
	  thisRunningExpenseTotal += numberFormat(thisForm['submitted[lender_investment_formula__budget][monthly_obligations][monthly_amount][installment_loan_2]'].value);
  
	  thisRunningExpenseTotal += numberFormat(thisForm['submitted[lender_investment_formula__budget][monthly_obligations][monthly_amount][installment_loan_3]'].value);
  
	  thisRunningExpenseTotal += numberFormat(thisForm['submitted[lender_investment_formula__budget][monthly_obligations][monthly_amount][installment_loan_4]'].value);
  
  
	  thisRunningExpenseTotal += numberFormat(thisForm['submitted[lender_investment_formula__budget][monthly_obligations][monthly_amount][credit_card_1]'].value);
  
	  thisRunningExpenseTotal += numberFormat(thisForm['submitted[lender_investment_formula__budget][monthly_obligations][monthly_amount][credit_card_2]'].value);
  
	  thisRunningExpenseTotal += numberFormat(thisForm['submitted[lender_investment_formula__budget][monthly_obligations][monthly_amount][credit_card_3]'].value);
  
	  thisRunningExpenseTotal += numberFormat(thisForm['submitted[lender_investment_formula__budget][monthly_obligations][monthly_amount][credit_card_4]'].value);
  
  
	  thisRunningExpenseTotal += numberFormat(thisForm['submitted[lender_investment_formula__budget][monthly_obligations][monthly_amount][other_1]'].value);
  
	  thisRunningExpenseTotal += numberFormat(thisForm['submitted[lender_investment_formula__budget][monthly_obligations][monthly_amount][other_2]'].value);
  
	  thisRunningExpenseTotal += numberFormat(thisForm['submitted[lender_investment_formula__budget][monthly_obligations][monthly_amount][other_3]'].value);
  

  thisForm['submitted[lender_investment_formula__budget][total_monthly_obligations]'].value = thisRunningExpenseTotal;

  if( numberFormat(thisForm['submitted[lender_investment_formula__budget][total_monthly_gross_income]'].value) == 0) {
	  thisForm['submitted[lender_investment_formula__budget][current_debt_ratio_line_2__line_1]'].value = - numberFormat(thisForm['submitted[lender_investment_formula__budget][total_monthly_obligations]'].value);			
  } else {
	  thisForm['submitted[lender_investment_formula__budget][current_debt_ratio_line_2__line_1]'].value = numberFormat(thisForm['submitted[lender_investment_formula__budget][total_monthly_obligations]'].value) / numberFormat(thisForm['submitted[lender_investment_formula__budget][total_monthly_gross_income]'].value);
  }
  tempNum = numberFormat(thisForm['submitted[lender_investment_formula__budget][current_debt_ratio_line_2__line_1]'].value);
  thisForm['submitted[lender_investment_formula__budget][current_debt_ratio_line_2__line_1]'].value = tempNum.toFixed(2);
  
  thisForm['submitted[lender_investment_formula__budget][total_amount_available_for_monthly_obligation]'].value = numberFormat(thisForm['submitted[lender_investment_formula__budget][total_monthly_gross_income]'].value) * .41;
  tempNum = numberFormat(thisForm['submitted[lender_investment_formula__budget][total_amount_available_for_monthly_obligation]'].value);
  thisForm['submitted[lender_investment_formula__budget][total_amount_available_for_monthly_obligation]'].value = tempNum.toFixed(2);

  thisForm['submitted[lender_investment_formula__budget][maximum_amount_available_for_housing]'].value = numberFormat(thisForm['submitted[lender_investment_formula__budget][total_amount_available_for_monthly_obligation]'].value) - numberFormat(thisForm['submitted[lender_investment_formula__budget][total_monthly_obligations]'].value);
  tempNum = numberFormat(thisForm['submitted[lender_investment_formula__budget][maximum_amount_available_for_housing]'].value);
  thisForm['submitted[lender_investment_formula__budget][maximum_amount_available_for_housing]'].value = tempNum.toFixed(2);

  thisForm['submitted[lender_investment_formula__budget][net_amount_available_for_house_payment]'].value = numberFormat(thisForm['submitted[lender_investment_formula__budget][maximum_amount_available_for_housing]'].value) - numberFormat(thisForm['submitted[lender_investment_formula__budget][monthly_lot_rent]'].value);
  tempNum = numberFormat(thisForm['submitted[lender_investment_formula__budget][net_amount_available_for_house_payment]'].value);
  thisForm['submitted[lender_investment_formula__budget][net_amount_available_for_house_payment]'].value = tempNum.toFixed(2);
}

$(document).ready(function() {
	$("form#webform-client-form-45-1").attr("name","worksheet");
	$("form#webform-client-form-45-1 input").change(function() {
		calculateBudget();
	});	
});