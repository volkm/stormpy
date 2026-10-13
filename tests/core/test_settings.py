import stormpy


class TestSettings:
    def test_log_level(self):
        previous = stormpy.developer.get_log_level()
        stormpy.set_log_level(stormpy.LogLevel.DEBUG)
        assert stormpy.developer.get_log_level() == stormpy.LogLevel.DEBUG
        stormpy.set_log_level(previous)

    def test_timeout(self):
        stormpy.set_timeout(1000)
        stormpy.reset_timeout()
