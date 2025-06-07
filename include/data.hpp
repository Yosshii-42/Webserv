#pragma once

enum HttpStatus {
	// 1xx Informational
	tContinue = 100,
	tSwitchingProtocols = 101,
	tProcessing = 102,

	// 2xx Success
	tOK = 200,
	tCreated = 201,
	tAccepted = 202,
	tNoContent = 204,

	// 3xx Redirection
	tMultipleChoices = 300,
	tMovedPermanently = 301,
	tFound = 302,
	tSeeOther = 303,
	tNotModified = 304,
	tTemporaryRedirect = 307,
	tPermanentRefirect = 308,

	// 4xx Client Error
	tBadRequest = 400,
	tUnauthorized = 401,
	tPaymentRequired = 402,
	tForbidden = 403,
	tNotFound = 404,
	tMethodNotAllowed = 405,
	tNotAccebptable = 406,
	tRequestTimeout = 408,
	tConflict = 409,
	tGone = 410,
	tPayloadTooLarge = 413,
	tExpectationFailed = 417,
	tLocked = 423,
	tUpgradeRequired = 426,
	tTooManyRequests = 429,

	// 5xx Server Error
	tInternalServerError = 500,
	tNotImplemented = 501,
	tBadGateway = 502,
	tServiceUnavailable = 503,
	tGatewayTimeout = 504,
	tHTTPVersionNotSupported = 505
};
