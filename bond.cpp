// This must be the last included header
#include "qlUtils.h"

extern "C" {

	K duration(K ytm, K valueDate, K maturityDate, K coupon, K frequency, K calendarStr, K durationStr)
	{
		QuantLib::Duration::Type durationType;
		QuantLib::Calendar calendar;
		QuantLib::Frequency couponFreq;

		try {
			checkType(ytm, -KF);
			checkDate(valueDate);
			checkDate(maturityDate);
			checkType(coupon, -KF);
			checkType(frequency, -KJ);
			checkType(calendarStr, KC, -KS);
			checkType(durationStr, KC, -KS);
			durationType = getDurationType(kdb2str(durationStr));
			calendar = getCalendar(kdb2str(calendarStr));
			couponFreq = getFrequency(frequency->j);
		}
		catch (std::exception &e) {
			return exception2krr(e);
		}
		
		QuantLib::Date valueDt = kDate2QL(valueDate);
		QuantLib::Date maturityDt = kDate2QL(maturityDate);

		if (maturityDt <= valueDt) 
			return krr((S)"Maturity date can't be less than value date");

		QuantLib::Compounding compounding;
		if (frequency->j == 0 && durationType != QuantLib::Duration::Macaulay)
		    compounding = QuantLib::Simple;
		else
			compounding = QuantLib::Compounded;
		
		std::vector<QuantLib::Rate> coupons = std::vector<QuantLib::Rate>(1, coupon->f);

		QuantLib::Schedule schedule(valueDt, maturityDt, QuantLib::Period(couponFreq), calendar, QuantLib::ModifiedFollowing, QuantLib::ModifiedFollowing, QuantLib::DateGeneration::Backward, false);

		QuantLib::FixedRateBond bond = QuantLib::FixedRateBond(2, 1000, schedule, coupons, QuantLib::ActualActual(QuantLib::ActualActual::Bond), QuantLib::BusinessDayConvention::ModifiedFollowing);

		QuantLib::InterestRate yield = QuantLib::InterestRate(ytm->f, QuantLib::ActualActual(QuantLib::ActualActual::Bond), compounding, couponFreq);

		return kf(QuantLib::BondFunctions::duration(bond, yield, durationType, valueDt));
	}


	K convexity(K ytm, K valueDate, K maturityDate, K coupon, K frequency, K calendarStr)
	{
		QuantLib::Calendar calendar;
		QuantLib::Frequency couponFreq;

		try {
			checkType(ytm, -KF);
			checkDate(valueDate);
			checkDate(maturityDate);
			checkType(coupon, -KF);
			checkType(frequency, -KJ);
			checkType(calendarStr, KC, -KS);
			calendar = getCalendar(kdb2str(calendarStr));
			couponFreq = getFrequency(frequency->j);
		}
		catch (std::exception &e) {
			return exception2krr(e);
		}

		QuantLib::Date valueDt = kDate2QL(valueDate);
		QuantLib::Date maturityDt = kDate2QL(maturityDate);

		if (maturityDt <= valueDt)
			return krr((S)"Maturity date can't be less than value date");

		QuantLib::Compounding compounding;
		if (frequency->j == 0)
			compounding = QuantLib::Simple;
		else
			compounding = QuantLib::Compounded;

		std::vector<QuantLib::Rate> coupons = std::vector<QuantLib::Rate>(1, coupon->f);

		QuantLib::Schedule schedule(valueDt, maturityDt, QuantLib::Period(couponFreq), calendar, QuantLib::ModifiedFollowing, QuantLib::ModifiedFollowing, QuantLib::DateGeneration::Backward, false);

		QuantLib::FixedRateBond bond = QuantLib::FixedRateBond(2, 1000, schedule, coupons, QuantLib::ActualActual(QuantLib::ActualActual::Bond), QuantLib::BusinessDayConvention::ModifiedFollowing);

		QuantLib::InterestRate yield = QuantLib::InterestRate(ytm->f, QuantLib::ActualActual(QuantLib::ActualActual::Bond), compounding, couponFreq);

		return kf(QuantLib::BondFunctions::convexity(bond, yield, valueDt));
	}


	K yield(K price, K valueDate, K maturityDate, K coupon, K frequency, K calendarStr)
	{
		QuantLib::Calendar calendar;
		QuantLib::Frequency couponFreq;

		try {
			checkType(price, -KF);
			checkDate(valueDate);
			checkDate(maturityDate);
			checkType(coupon, -KF);
			checkType(frequency, -KJ);
			checkType(calendarStr, KC, -KS);
			calendar = getCalendar(kdb2str(calendarStr));
			couponFreq = getFrequency(frequency->j);
		}
		catch (std::exception &e) {
			return exception2krr(e);
		}

		QuantLib::Date valueDt = kDate2QL(valueDate);
		QuantLib::Date maturityDt = kDate2QL(maturityDate);

		if (maturityDt <= valueDt)
			return krr((S)"Maturity date can't be less than value date");

		QuantLib::Compounding compounding;
		if (frequency->j == 0)
			compounding = QuantLib::Simple;
		else
			compounding = QuantLib::Compounded;

		std::vector<QuantLib::Rate> coupons = std::vector<QuantLib::Rate>(1, coupon->f);

		QuantLib::Schedule schedule(valueDt, maturityDt, QuantLib::Period(couponFreq), calendar, QuantLib::ModifiedFollowing, QuantLib::ModifiedFollowing, QuantLib::DateGeneration::Backward, false);

		QuantLib::FixedRateBond bond = QuantLib::FixedRateBond(2, 1000, schedule, coupons, QuantLib::ActualActual(QuantLib::ActualActual::Bond), QuantLib::BusinessDayConvention::ModifiedFollowing, 100);

		return kf(QuantLib::BondFunctions::yield(QuantLib::Brent(), bond, price->f, QuantLib::ActualActual(QuantLib::ActualActual::Bond), compounding, couponFreq, valueDt));
	}
}