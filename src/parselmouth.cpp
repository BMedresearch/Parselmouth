/*
 * Copyright (C) 2016-2017  Yannick Jadoul
 *
 * This file is part of Parselmouth.
 *
 * Parselmouth is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Parselmouth is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Parselmouth.  If not, see <http://www.gnu.org/licenses/>
 */

#include "parselmouth/Parselmouth.h"
#include "version.h"

#include <praat/sys/praat.h>
#include <praat/sys/praat_version.h>

#define XSTR(s) STR(s)
#define STR(s) #s

namespace py = pybind11;
using namespace py::literals;

namespace parselmouth {

enum class Interpolation // TODO Remove/move to header
{
	NEAREST = Vector_VALUE_INTERPOLATION_NEAREST,
	LINEAR = Vector_VALUE_INTERPOLATION_LINEAR,
	CUBIC = Vector_VALUE_INTERPOLATION_CUBIC,
	SINC70 = Vector_VALUE_INTERPOLATION_SINC70,
	SINC700 = Vector_VALUE_INTERPOLATION_SINC700
};

}

using namespace parselmouth;


PYBIND11_MODULE(parselmouth, m) {
	praatlib_init();

	parselmouth::PraatBindings bindings(m);

    static py::exception<MelderError> melderErrorException(m, "PraatError", PyExc_RuntimeError);
	py::register_exception_translator([](std::exception_ptr p) {
			try {
				if (p) std::rethrow_exception(p);
			}
			catch (const MelderError &) { // TODO Unicode encoding?
				std::string message(Melder_peek32to8(Melder_getError()));
				message.erase(message.length() - 1);
				Melder_clearError();
				melderErrorException(message.c_str());
			}});

	m.attr("__version__") = PYBIND11_STR_TYPE(XSTR(PARSELMOUTH_VERSION));
	m.attr("VERSION") = py::str(XSTR(PARSELMOUTH_VERSION));
	m.attr("PRAAT_VERSION") = py::str(XSTR(PRAAT_VERSION_STR));
	m.attr("PRAAT_VERSION_DATE") = py::str(XSTR(PRAAT_DAY) " " XSTR(PRAAT_MONTH) " " XSTR(PRAAT_YEAR));

	bindings.init();

	bindings.get<Intensity>()
		.def("get_value", // TODO Should be part of Vector class
				[] (Intensity self, double time, Interpolation interpolation) { return Vector_getValueAtX(self, time, 1, static_cast<int>(interpolation)); },
				"time"_a, "interpolation"_a = Interpolation::CUBIC)
	;

	bindings.get<Harmonicity>()
		.def("get_value", // TODO Should be part of Vector class
				[] (Harmonicity self, double time, Interpolation interpolation) { return Vector_getValueAtX(self, time, 1, static_cast<int>(interpolation)); },
				"time"_a, "interpolation"_a = Interpolation::CUBIC)
	;
}
