// JavaScript Document

var bDebug = 0 ;        // change to 1 for extra output
var sNl = "\r";
var sBlanks = "                                             "
                + "                                          " ;
var sDashes = "---------------------------------------------"
                + "-----------------------------------------" ;

var nPowers = [
        0.0000000000000001,
        0.000000000000001,
        0.00000000000001,
        0.0000000000001,
        0.000000000001,
        0.00000000001,
        0.0000000001,
        0.000000001,
        0.00000001,
        0.0000001,
        0.000001,
        0.00001,
        0.0001,
        0.001,
        0.01,
        0.1,
        1.,
        10.,
        100.,
        1000.,
        10000.,
        100000.,
        1000000.,
        10000000.,
        100000000.,
        1000000000.,
        10000000000.] ;

function Power(n)
{
    return nPowers[n+16] ;
}

var iTotalWidth = 70 ;
var iColumns = [5, 15, 15, 15, 15] ;
var sHeadings1 = ["#", "Payment", "Interest", "Principal", "Balance"] ;
var sHeadings2 = ["", "Amount", "Amount", "Reduction", "Due"] ;

function Execute(oForm)
{
   var iError = 0 ;
   var sOut = CenterString("Loan Repayment Schedule", iTotalWidth) + sNl
        + sDashes.substring(0, iTotalWidth) + sNl ;
   var nPrincipal = parseFloat(oForm['submitted[amt]'].value) ;
   var nInputPercent = parseFloat(oForm['submitted[per]'].value) ;
   var nPercent = nInputPercent / 1200 ;
   var iMonths = Math.round(parseFloat(oForm['submitted[years]'].value) * 12) ;
//   var iMonths = parseInt(oForm.years.value) * 12 ;
   var nPayment = parseFloat(oForm['submitted[payment]'].value) ;


// if (nPrincipal < 0 || nPercent < 0 || iMonths < 0 || nPayment < 0)
// {
//    iError = 100 ;
//    sOut += "Error: No fields may be negative." + sNl ;
// }

//   if (nPercent < 0 || iMonths < 0)
   if (iMonths < 0)
   {
      iError = 100 ;
      sOut += "Error: Percent and years may not be negative." + sNl ;
   }

   var iCount = (nPrincipal ? 1 : 0) + (nPercent ? 1 : 0)
                        + (iMonths ? 1 : 0) + (nPayment ? 1 : 0) ;
   if (iCount < 3)
   {
      iError = 110 ;
      sOut += "Error: At most one field must be set to zero." ;
   }

   var nX, nY, iCount = 25 ;

   if (!iError)
   {
      if (!iMonths)             // number of years not specified
      {
         if (nPercent * nPrincipal >= nPayment)
         {
            iError = 200 ;
            sOut += "Error: Loan payment amount is too small "
                        + "to repay loan." + sNl ;
         }

         else
         {
            iMonths = Math.floor(Math.log(1-nPercent*nPrincipal / nPayment)
                        / Math.log(1 - nPercent)) - 1 ;
            if (iMonths < 1)
               iMonths = 1 ;
            while (iCount-- > 0 && (nX = PresentValue(nPayment,
                                        nPercent, iMonths)) < nPrincipal)
            {
               if (bDebug)
                  sOut += "iMonths, nX: " + iMonths + ", " + nX + sNl ;
               ++iMonths ;
            }
         }
      }

      if (!nPercent)
      {
         var nX = 1 ;
         if (PresentValue(nPayment, 0, iMonths) < nPrincipal)
         {
            iError = 210 ;
            sOut += "Error: Loan payment amount is too small "
                        + "to repay loan." + sNl ;
         }

         else
         {
            while (iCount-- > 0 && Math.abs(nPrincipal
                - (nPv = PresentValue(nPayment, nX, iMonths))) > .005)
            {
               if (bDebug)
                  sOut += "nX, nPv: " + nX + ", " + nPv + sNl ;
               nX *= 1 + (nPv - nPrincipal) / nPrincipal ;
            }

            if (bDebug)
               sOut += "Exit: nX, nPv: " + nX + ", " + nPv + sNl ;

            nInputPercent = Round(1200 * nX - .00005, 4) ;
            nPercent = nInputPercent / 1200 ;
         }
      }

      if (!nPrincipal)
      {
         nPrincipal = PresentValue(nPayment, nPercent, iMonths) ;
         nPrincipal = Round(nPrincipal, 2) ;
      }

      if (!nPayment)
      {
         nPayment = nPrincipal / PresentValue(1, nPercent, iMonths) ;
         nPayment = Round(nPayment + .005, 2) ;
      }
   }

   if (!iError && iMonths > 12000)
   {
      iError = 250 ;
      sOut += "Error: Can't print out schedule for more than "
                        + "12000 months." + sNl ;
   }

   if (!iError)
   {
      sOut += "Repay $" + nPrincipal + " at " + nInputPercent
                + "% annual interest in "
                + iMonths + " months:" + sNl
                + "     Monthly Payment = $" + nPayment
                + sNl + sDashes.substring(0,iTotalWidth) + sNl ;

      for (var iRow = 1 ; iRow <= 2 ; ++iRow)
      {
         for (var iCol = 0 ; iCol < 5 ; ++iCol)
            sOut += PrepadString( iRow == 1 ? sHeadings1[iCol]
                     : sHeadings2[iCol], iColumns[iCol]) ;
         sOut += sNl ;
      }

      sOut += sDashes.substring(0, iTotalWidth) + sNl ;

      var nInterest, nReduction, nBalance = nPrincipal ;
      var nFraction = 0, nTotalInterest = 0 ;

      for (var iK = 1 ; iK <= iMonths ; ++iK)
      {
         nInterest = nBalance * nPercent + nFraction ;
	 nFraction = nInterest - Round(nInterest,2) ;
	 nInterest = Round(nInterest,2) ;
         if (nPayment > nBalance + nInterest)
            nPayment = nBalance + nInterest ;
         nReduction = nPayment - nInterest ;
         nBalance -= nReduction ;
	 nTotalInterest += nInterest ;

         sOut += Format(iK, iColumns[0], 0)
                        + DollarFormat(nPayment, iColumns[1], 2) ;
         sOut += DollarFormat(nInterest, iColumns[2], 2) ;
         sOut += DollarFormat(nReduction, iColumns[3], 2) ;
         sOut += DollarFormat(nBalance, iColumns[4], 2) + sNl ;
      }

      sOut += sNl + "Future value of loan amount = "
		+ DollarFormat(FutureValue(nPrincipal,
				nPercent, iMonths), 0, 2) + sNl ;
      sOut += sNl + "Total interest paid = "
      		+ DollarFormat(nTotalInterest, 0, 2) ;
   }

// oForm.sched.rows = 1000 ;
   oForm['submitted[sched]'].rows = iError ? 20 : 
                (iMonths < 500) ? (iMonths + 12) : 500 ;
// alert(oForm.sched.ROWS) ;
// alert(oForm.sched.toString()) ;
   oForm['submitted[sched]'].value = sOut ;

}

function ExpN(nX, iN)   // compute x ** n, where n is integral
{
   var nResult = 1 ;
   var bSign = 0 ;
   if (iN < 0)
   {
      bSign = 1 ;
      iN = -iN ;
   }
   while (iN > 0)
   {
      if (iN & 1)
         nResult *= nX ;
      nX *= nX ;
      iN >>= 1 ;
   }
   if (bSign)
      nResult = 1 / nResult ;
   return nResult ;
}

function PresentValue(nPayment, nPercent, iNumPeriods)
{
   var nAmount = (Math.abs(nPercent) > 1e-20)
         ? nPayment * (1 - ExpN(1 + nPercent, -iNumPeriods))
      					/ nPercent 
         : nPayment * iNumPeriods ;
    return nAmount ;
}

function FutureValue(nPayment, nPercent, iNumPeriods)
{
   var nAmount = nPayment * ExpN(1 + nPercent, iNumPeriods) ;
   return nAmount ;
}

function Round(nVal, iD)
{
   var iSign = 1 ;
   if (nVal < 0)
   {
      nVal = - nVal ;
      iSign = -1 ;
   }
   var iInt = Math.round(nVal) ;
   if (iD > 0)
      iInt = Math.floor(nVal) ;
   var nFp = nVal - iInt ;
// alert ('iInt, nFp = ' + iInt + ", " + nFp) ;
   if (iD > 0)
      nFp = Math.round(nFp * Power(iD)) / Power(iD) ;
   nVal = iSign * (iInt + nFp) ;
   return nVal ;
}

// DollarFormat -- could be jazzed up to produce "CR" or "DB"
function DollarFormat(nVal, iW, iD)
{
   return GenFmt(nVal, iW, iD, 1) ;
}

function Format(nVal, iW, iD)
{
   return GenFmt(nVal, iW, iD, 0) ;
}

function GenFmt(nVal, iW, iD, bDollar) // format val into w chars,
			// d digs after decimal point
{
   var sOut = "" ;
   var iSign = 0 ;
   if (nVal < 0)
   {
      nVal = - nVal ;
      iSign = 1 ;
   }
   var iInt = Math.round(nVal) ;
   if (iD > 0)
      iInt = Math.floor(nVal) ;
   var nFp = nVal - iInt ;
   var iDigs = 1 ;
   if (iInt > 9)
      iDigs = Math.floor(Math.log(iInt+.1)/Math.log(10)) + 1 ;
   var iLeft = iW - iSign - (bDollar ? 1 : 0) ;
   if (iD > 0)
      iLeft -= iD + 1 ;
   if (iLeft > iDigs)
      sOut += sBlanks.substring(0, iLeft - iDigs) ;
   if (iSign)
      sOut += '-' ;
   if (bDollar)
      sOut += '$' ;
   sOut += iInt ;
   if (iD > 0)
   {
      nFp = Math.round((1 + nFp) * Power(iD)) ;
      sOut += '.' + String(nFp).substring(1) ;
   }
   return sOut ;
}

function PrepadString(sStr, iW)
{
   if (sStr.length < iW)
      sStr = sBlanks.substring(0, iW - sStr.length) + sStr ;
   return sStr ;
}

function CenterString(sStr, iW)
{
   var iBlanks = iW - sStr.length ;
   if (iBlanks > 0)
      sStr = sBlanks.substring(0, Math.floor(iBlanks/2)) + sStr
                + sBlanks.substring(0, iBlanks - Math.floor(iBlanks/2)) ;
   return sStr ;
}

//-->

// Separate from the original
$(document).ready(function() {
	$("form#webform-client-form-101").submit(function() {
		Execute(this);
		$("form#webform-client-form-101 div#webform-component-sched").fadeIn(1000);
		return false;
	});
});