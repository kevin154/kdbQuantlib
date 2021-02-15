#pragma once
#include <ql/quantlib.hpp>
// This must be the last included header
#include "kutils.h"

// Map from C++ dates (starting 1900.01.01) to kdb+ dates (starting 2000.01.01)
#define KDB_DATE_ADJUSTMENT 36526


static QuantLib::Date kDate2QL(K kDate) 
{
	return QuantLib::Date(kDate->i + KDB_DATE_ADJUSTMENT);
}


static QuantLib::Calendar getCalendar(std::string calendarID)
{
	if (calendarID == "Unadjusted" || calendarID == "")
		return QuantLib::NullCalendar();
	else if (calendarID == "Weekends")
		return QuantLib::WeekendsOnly();
	else if (calendarID == "TARGET")
		return QuantLib::TARGET();
	else if (calendarID == "United States")
		return QuantLib::UnitedStates();
	else if (calendarID == "Japan")
		return QuantLib::Japan();
	else if (calendarID == "United Kingdom")
		return QuantLib::UnitedKingdom();
	else if (calendarID == "Switzerland")
		return QuantLib::Switzerland();
	else if (calendarID == "Canada")
		return QuantLib::Canada();
	else if (calendarID == "Singapore")
		return QuantLib::Singapore();
	else if (calendarID == "Hong Kong")
		return QuantLib::HongKong();
	else if (calendarID == "China")
		return QuantLib::China();
	else if (calendarID == "Australia")
		return QuantLib::Australia();
	else if (calendarID == "New Zealand")
		return QuantLib::NewZealand();
	else if (calendarID == "Sweden")
		return QuantLib::Sweden();
	else if (calendarID == "Denmark")
		return QuantLib::Denmark();
	else if (calendarID == "Norway")
		return QuantLib::Norway();
	else if (calendarID == "Czech Republic")
		return QuantLib::CzechRepublic();
	else if (calendarID == "Poland")
		return QuantLib::Poland();
	else if (calendarID == "South Africa")
		return QuantLib::SouthAfrica();
	else if (calendarID == "Hungary")
		return QuantLib::Hungary();
	// Throw error if not implemented
	else throw std::runtime_error("Unsupported calendar: " + calendarID);
}


static QuantLib::BusinessDayConvention getRollConvention(std::string busDayConvention)
{
	if (busDayConvention == "Modified Following")
		return QuantLib::ModifiedFollowing;
	else if (busDayConvention == "Following")
		return QuantLib::Following;
	else if (busDayConvention == "Modified Preceding")
		return QuantLib::ModifiedPreceding;
	else if (busDayConvention == "Preceding")
		return QuantLib::Preceding;
	else if (busDayConvention == "Unadjusted")
		return QuantLib::Unadjusted;
	// Throw error if not implemented
	else throw std::runtime_error("Unsupported day roll convention: " + busDayConvention);
}


static QuantLib::DayCounter getDayCountConvention(std::string dayCountConvention)
{
	if (dayCountConvention == "ACT/360")
		return QuantLib::Actual360();
	else if (dayCountConvention == "ACT/365.FIXED")
		return QuantLib::Actual365Fixed();
	else if (dayCountConvention == "30/360")
		return QuantLib::Thirty360();
	else if (dayCountConvention == "30E/360")
		return QuantLib::Thirty360(QuantLib::Thirty360::European);
	else if (dayCountConvention == "30E/360.ISDA")
		return QuantLib::Thirty360(QuantLib::Thirty360::German);
	else if (dayCountConvention == "30E/360.ISDA")
		return QuantLib::Thirty360(QuantLib::Thirty360::European);
	else if (dayCountConvention == "ACT/ACT.ISDA")
		return QuantLib::ActualActual(QuantLib::ActualActual::ISDA);
	else if (dayCountConvention == "ACT/ACT.ISMA")
		return QuantLib::ActualActual(QuantLib::ActualActual::ISMA);
	// ICMA is an alias of ISMA
	else if (dayCountConvention == "ACT/ACT.ICMA")
		return QuantLib::ActualActual(QuantLib::ActualActual::ISMA);
	else if (dayCountConvention == "1/1")
		return QuantLib::OneDayCounter();
	// Throw error if not implemented
	else throw std::runtime_error("Unsupported day count convention: " + dayCountConvention);
}


static QuantLib::Duration::Type getDurationType(std::string durationStr) 
{
	if (durationStr == "Simple")
		return QuantLib::Duration::Simple;
	else if (durationStr == "Modified")
		return QuantLib::Duration::Modified;
	else if (durationStr == "Macauley")
		return QuantLib::Duration::Macaulay;
	// Default behaviour is to return modified duration
	else if (durationStr == "")
		return QuantLib::Duration::Modified;
	// Throw error if not implemented
	else throw std::runtime_error("Unsupported duration type: " + durationStr);
}


static QuantLib::Frequency getFrequency(long long freq) 
{
	switch (freq) 
	{
	case 0:
		return QuantLib::Frequency::Once;
	case 1:
		return QuantLib::Frequency::Annual;
	case 2:
		return QuantLib::Frequency::Semiannual;
	case 4:
	    return QuantLib::Frequency::Quarterly;
	case 6:
		return QuantLib::Frequency::Bimonthly;
	case 12:
		return QuantLib::Frequency::Monthly;
	case 26:
		return QuantLib::Frequency::Biweekly;
	case 52:
		return QuantLib::Frequency::Weekly;
	case 365:
		return QuantLib::Frequency::Daily;
	// Throw error if not implemented
	default: 
		throw std::runtime_error("Unsupported frequency: " + std::to_string(freq));
	}		
}