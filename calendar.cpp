// This must be the last included header
#include "qlUtils.h"

extern "C" {

	K countBusinessDays(K calendarID, K startDate, K endDate)
	{
		QuantLib::Calendar calendar;
		try {
			checkType(calendarID, KC, -KS);
			checkDate(startDate);
			checkDate(endDate);
			calendar = getCalendar(kdb2str(calendarID));
		}
		catch (std::exception &e) {
			return exception2krr(e);
		}
		if (endDate->i <= startDate->i) 
			return kj(0);
		return kj(calendar.businessDaysBetween(kDate2QL(startDate), kDate2QL(endDate)));
	}


	K getBusinessDays(K calendarID, K startDate, K endDate)
	{
		QuantLib::Calendar calendar;
		try {
			checkType(calendarID, KC, -KS);
			checkDate(startDate);
			checkDate(endDate);
			calendar = getCalendar(kdb2str(calendarID));
		}
		catch (std::exception &e) {
			return exception2krr(e);
		}
		int startDt = startDate->i, endDt = endDate->i;
		// If start date less than end date return null date
		if (endDt < startDt) return kd(ni);

		K businessDays = ktn(KD, 0);
		for (int i = startDt; i <= endDt; i++) {
			if (calendar.isBusinessDay(QuantLib::Date(i + KDB_DATE_ADJUSTMENT)))
				ja(&businessDays, &i);
		}
		return businessDays;
	}


	K getHolidays(K calendarID, K startDate, K endDate, K includeWeekends)
	{
		QuantLib::Calendar calendar;
		try {
			checkType(calendarID, KC, -KS);
			checkDate(startDate);
			checkDate(endDate);
			checkType(includeWeekends, -KB);
			calendar = getCalendar(kdb2str(calendarID));
		}
		catch (std::exception &e) {
			return exception2krr(e);
		}

		// If start date less than end date return null date
		if (endDate->i < startDate->i) return kd(ni);

		// Generate list of dates in QuantLib format
		std::vector<QuantLib::Date> holList =
			calendar.holidayList(kDate2QL(startDate), kDate2QL(endDate), includeWeekends->g);

		// Container for list of holidays in kdb+ format
		K holidays = ktn(KD, 0);

		// Underlying integer for a particular date
		int dateInt;

		// Loop over vector and join to kdb+ holiday list
		for (auto dt : holList)
		{
			dateInt = dt.serialNumber() - KDB_DATE_ADJUSTMENT;
			ja(&holidays, &dateInt);
		}
		return holidays;
	}


	K getIMMDates(K calendarID, K startDate, K endDate, K rollConvention) 
	{
		QuantLib::Calendar calendar;
		QuantLib::BusinessDayConvention dayRollRule;
		try {
			checkType(calendarID, KC, -KS);
			checkDate(startDate);
			checkDate(endDate);
			checkType(rollConvention, KC, -KS);
			calendar = getCalendar(kdb2str(calendarID));
			dayRollRule = getRollConvention(kdb2str(rollConvention));
		}
		catch (std::exception &e) {
			return exception2krr(e);
		}

		QuantLib::Date startDt(kDate2QL(startDate)), endDt(kDate2QL(endDate));

		// If start date less than end date return null date
		if (endDt < startDt) return kd(ni);

		// Empty list to hold IMM dates
		K immDates = ktn(KD, 0);
		
		// Underlying integer for a particular date
		int dateInteger;

		// Initialise loop on the start date if it is an IMM date, else start at the next IMM date
		QuantLib::Date refDt = QuantLib::IMM::nextDate(startDt - 1);

		// Roll forward to next IMM date and adjust as per day roll convention
		while (refDt <= endDt) {
			dateInteger = calendar.adjust(refDt, dayRollRule).serialNumber() - KDB_DATE_ADJUSTMENT;
			ja(&immDates, &dateInteger);
			refDt = QuantLib::IMM::nextDate(refDt);
		}
		return immDates;
	}


	K yearFrac(K startDate, K endDate, K dayCountConvention) 
	{
		QuantLib::DayCounter dc;
		try {
			checkDate(startDate);
			checkDate(endDate);
			checkType(dayCountConvention, -KS, KC);
			dc = getDayCountConvention(kdb2str(dayCountConvention));
		}
		catch (std::exception &e) {
			return exception2krr(e);
		}
		return kf(dc.yearFraction(kDate2QL(startDate), kDate2QL(endDate)));
	}


	K rollDates(K dateList, K calendarID, K rollConvention)
	{
		QuantLib::Calendar calendar;
		QuantLib::BusinessDayConvention dayRollRule;
		try {
			checkType(dateList, KD);
			checkType(calendarID, KC, -KS);
			checkType(rollConvention, KC, -KS);
			calendar = getCalendar(kdb2str(calendarID));
			dayRollRule = getRollConvention(kdb2str(rollConvention));
		}
		catch (std::exception &e) {
			return exception2krr(e);
		}

		// Define variables for the running date int and K list length
		int dateInt; 
		long long countList = dateList->n;

		// Container for return value, a list of rolled dates
		K rolledDates = ktn(KD, countList);

		// Define QuantLib date
		QuantLib::Date QLdate;

		for (long i = 0; i < countList; i++) {
			
			dateInt = kI(dateList)[i];
			
			// Check for null dates  
			try {
				checkDate(kd(dateInt));
			}
			catch (std::exception &e) {
				return exception2krr(e);
			}
			// Convert kdb+ date to QuantLib date format  
			QuantLib::Date QLdate(dateInt + KDB_DATE_ADJUSTMENT);

			// Roll date and convert back to kdb+ date
			dateInt = calendar.adjust(QLdate, dayRollRule).serialNumber() - KDB_DATE_ADJUSTMENT;

			// Add to the kdb+ date list
			kI(rolledDates)[i] = dateInt;
		}
		return rolledDates;
	}
}