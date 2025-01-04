if (std::holds_alternative<HandlerWithoutParameters>(handlerVariant))
{
	return get<HandlerWithoutParameters>(handlerVariant);
}
else
{
	return std::bind(std::get<HandlerWithParameters>(handlerVariant),
					 std::placeholders::_1,
					 std::placeholders::_2,
					 params);
}



