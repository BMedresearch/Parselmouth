import pytest

import parselmouth


def test_run(resources):
	script = """
	Read from file: "{}"
	To Intensity: 100.0, 0.0, "yes"
	selectObject: 1
	selectObject: "Intensity the_north_wind_and_the_sun"
	""".format(resources["the_north_wind_and_the_sun.wav"])

	assert parselmouth.praat.run(script)[0] == parselmouth.Sound(resources["the_north_wind_and_the_sun.wav"]).to_intensity()

	with pytest.raises(parselmouth.PraatError, match="Found 3 arguments but expected only 0."):
		parselmouth.praat.run(script, 42, "some_argument", True)


def test_run_with_parameters(resources):
	script = """
	form Test
		positive minPitch 100.0
		real timeStep 0.0
		boolean subtractMean "yes"
	endform
	
	Read from file: "{}"
	To Intensity: minPitch, timeStep, subtractMean
	selectObject: 1
	selectObject: "Intensity the_north_wind_and_the_sun"
	""".format(resources["the_north_wind_and_the_sun.wav"])

	min_pitch = 75.0
	time_step = 0.05
	subtract_mean = False

	assert parselmouth.praat.run(script, min_pitch, time_step, subtract_mean)[0] == parselmouth.Sound(resources["the_north_wind_and_the_sun.wav"]).to_intensity(min_pitch, time_step, subtract_mean)

	with pytest.raises(parselmouth.PraatError, match="Found 0 arguments but expected more."):
		parselmouth.praat.run(script)


def test_run_with_capture_output():
	assert parselmouth.praat.run("writeInfo: 42", capture_output=True) == ([], "42")
	assert parselmouth.praat.run("appendInfo: 42", capture_output=True) == ([], "42")
	assert parselmouth.praat.run("writeInfoLine: 42", capture_output=True) == ([], "42\n")
	assert parselmouth.praat.run("writeInfoLine: \"The answer\", \" - \", 42\nappendInfo: \"The question - ?\"", capture_output=True) == ([], "The answer - 42\nThe question - ?")
	assert parselmouth.praat.run("writeInfoLine: \"The answer\", \" - \", 42\nwriteInfoLine: \"The question - ?\"", capture_output=True) == ([], "The question - ?\n")
	assert parselmouth.praat.run("writeInfo: tab$, newline$", capture_output=True) == ([], "\t\n")


def test_with_return_variables():
	objects, variables = parselmouth.praat.run("a = 42\nb$ = \"abc\"", return_variables=True)
	assert objects == []
	assert 'a' in variables and 'a$' not in variables and isinstance(variables['a'], float) and variables['a'] == 42
	assert 'b$' in variables and 'b' not in variables and variables['b$'] == "abc"
	assert set(variables.keys()) == {'a', 'b$', 'newline$', 'tab$', 'shellDirectory$', 'defaultDirectory$', 'preferencesDirectory$', 'homeDirectory$', 'temporaryDirectory$', 'macintosh', 'windows', 'unix', 'left', 'right', 'mono', 'stereo', 'all', 'average', 'praatVersion$', 'praatVersion'}


def test_with_capture_output_and_return_variables():
	objects, output, variables = parselmouth.praat.run("a = 42\nb$ = \"abc\"\nwriteInfoLine: a\nappendInfoLine: b$", capture_output=True, return_variables=True)
	assert objects == []
	assert output == "42\nabc\n"
	assert 'a' in variables and 'b$' in variables

