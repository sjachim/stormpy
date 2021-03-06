import stormpy


class TestExpressions:
    def test_expression_manager(self):
        manager = stormpy.ExpressionManager()

    def test_boolean_expression(self):
        manager = stormpy.ExpressionManager()
        expression = manager.create_boolean(True)
        assert expression.is_literal()
        assert not expression.contains_variables()
        assert expression.has_boolean_type()
        assert not expression.has_integer_type()
        assert not expression.has_rational_type()

    def test_integer_expression(self):
        manager = stormpy.ExpressionManager()
        expression = manager.create_integer(2)
        assert expression.is_literal()
        assert not expression.contains_variables()
        assert not expression.has_boolean_type()
        assert expression.has_integer_type()
        assert not expression.has_rational_type()

    def test_rational_expression(self):
        manager = stormpy.ExpressionManager()
        expression = manager.create_rational(stormpy.Rational(0.2))
        assert expression.is_literal()
        assert not expression.contains_variables()
        assert not expression.has_boolean_type()
        assert not expression.has_integer_type()
        assert expression.has_rational_type()

    def test_expression_parser(self):
        manager = stormpy.ExpressionManager()
        ep = stormpy.ExpressionParser(manager)
        ep.set_identifier_mapping(dict())
        print(ep.parse("true"))
        print(ep.parse("1.0"))
        assert ep.parse("1.0").has_rational_type()
