void DefaultGlobalExceptionHandler(Request& req, Response& res, std::function<void()> routehandler) {
	try {
		// Handle request with middleware
		routehandler();
	}
	catch (const std::exception& e) {
		std::cerr << "Error: " << e.what() << std::endl;
		setResponseInternalServerError(res);
	}
	catch (...) {
		std::cerr << "Unspecified error occured" << std::endl;
		setResponseInternalServerError(res);
	}
}