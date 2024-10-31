"use strict";
var __awaiter = (this && this.__awaiter) || function (thisArg, _arguments, P, generator) {
    function adopt(value) { return value instanceof P ? value : new P(function (resolve) { resolve(value); }); }
    return new (P || (P = Promise))(function (resolve, reject) {
        function fulfilled(value) { try { step(generator.next(value)); } catch (e) { reject(e); } }
        function rejected(value) { try { step(generator["throw"](value)); } catch (e) { reject(e); } }
        function step(result) { result.done ? resolve(result.value) : adopt(result.value).then(fulfilled, rejected); }
        step((generator = generator.apply(thisArg, _arguments || [])).next());
    });
};
var __generator = (this && this.__generator) || function (thisArg, body) {
    var _ = { label: 0, sent: function() { if (t[0] & 1) throw t[1]; return t[1]; }, trys: [], ops: [] }, f, y, t, g = Object.create((typeof Iterator === "function" ? Iterator : Object).prototype);
    return g.next = verb(0), g["throw"] = verb(1), g["return"] = verb(2), typeof Symbol === "function" && (g[Symbol.iterator] = function() { return this; }), g;
    function verb(n) { return function (v) { return step([n, v]); }; }
    function step(op) {
        if (f) throw new TypeError("Generator is already executing.");
        while (g && (g = 0, op[0] && (_ = 0)), _) try {
            if (f = 1, y && (t = op[0] & 2 ? y["return"] : op[0] ? y["throw"] || ((t = y["return"]) && t.call(y), 0) : y.next) && !(t = t.call(y, op[1])).done) return t;
            if (y = 0, t) op = [op[0] & 2, t.value];
            switch (op[0]) {
                case 0: case 1: t = op; break;
                case 4: _.label++; return { value: op[1], done: false };
                case 5: _.label++; y = op[1]; op = [0]; continue;
                case 7: op = _.ops.pop(); _.trys.pop(); continue;
                default:
                    if (!(t = _.trys, t = t.length > 0 && t[t.length - 1]) && (op[0] === 6 || op[0] === 2)) { _ = 0; continue; }
                    if (op[0] === 3 && (!t || (op[1] > t[0] && op[1] < t[3]))) { _.label = op[1]; break; }
                    if (op[0] === 6 && _.label < t[1]) { _.label = t[1]; t = op; break; }
                    if (t && _.label < t[2]) { _.label = t[2]; _.ops.push(op); break; }
                    if (t[2]) _.ops.pop();
                    _.trys.pop(); continue;
            }
            op = body.call(thisArg, _);
        } catch (e) { op = [6, e]; y = 0; } finally { f = t = 0; }
        if (op[0] & 5) throw op[1]; return { value: op[0] ? op[1] : void 0, done: true };
    }
};
var __importDefault = (this && this.__importDefault) || function (mod) {
    return (mod && mod.__esModule) ? mod : { "default": mod };
};
Object.defineProperty(exports, "__esModule", { value: true });
var ioredis_1 = __importDefault(require("ioredis"));
var express_1 = __importDefault(require("express"));
var body_parser_1 = __importDefault(require("body-parser"));
var fs_1 = __importDefault(require("fs"));
var crypto_1 = __importDefault(require("crypto"));
// DEFINITIONS =================================================================
var JSON_CONTENT_TYPE = "application/json";
// LIB =========================================================================
function getWeatherData() {
    return __awaiter(this, void 0, void 0, function () {
        var items, weatherData, i;
        return __generator(this, function (_a) {
            switch (_a.label) {
                case 0: return [4 /*yield*/, redis.xrevrange("weather-station:0", "+", "-", "COUNT", 100)];
                case 1:
                    items = _a.sent();
                    weatherData = [];
                    for (i = 0; i < items.length; i++) {
                        weatherData[i] = {
                            timestamp: items[i][0],
                            temp_c: items[i][1][1],
                            humid_prcnt: items[i][1][3],
                            pressure_kpa: items[i][1][5],
                            is_raining: items[i][1][7],
                            light_level: items[i][1][9],
                        };
                    }
                    return [2 /*return*/, weatherData];
            }
        });
    });
}
// data -> JSON object containing the new weather station data.
function postWeatherData(weatherRecord) {
    return __awaiter(this, void 0, void 0, function () {
        return __generator(this, function (_a) {
            try {
                // Try to add stuff to redis database
                redis.xadd("weather-station:0", "MAXLEN", "~", "720", "*", "temp_c", "".concat(weatherRecord.temp_c), "humid_prcnt", "".concat(weatherRecord.humid_prcnt), "pressure_kpa", "".concat(weatherRecord.pressure_kpa), "is_raining", "".concat(weatherRecord.is_raining), "light_level", "".concat(weatherRecord.light_level));
            }
            catch (error) {
                return [2 /*return*/, false];
            }
            return [2 /*return*/, true];
        });
    });
}
function isJsonWeatherRecord(json) {
    return json.hasOwnProperty('timestamp')
        && json.hasOwnProperty('temp_c')
        && json.hasOwnProperty('humid_prcnt')
        && json.hasOwnProperty('pressure_kpa')
        && json.hasOwnProperty('is_raining')
        && json.hasOwnProperty('light_level');
}
function loadKeys(path) {
    try {
        var fileData = fs_1.default.readFileSync(path);
        var keys = fileData.toString().split("\n");
        if (keys.length == 0) {
            return null;
        }
        return keys;
    }
    catch (err) {
        console.log(err);
        return null;
    }
}
// GLOBALS =====================================================================
var app = (0, express_1.default)();
var port = 27500;
var keysPath = "./api.keys";
// API APP =====================================================================
var redis = new ioredis_1.default({
    port: 6379,
    host: "localhost",
    retryStrategy: function (times) {
        var delay = Math.min(times * 50, 2000);
        return delay;
    },
});
app.use(body_parser_1.default.json());
redis.on('error', function (e) {
    console.log("REDIS CLIENT ERROR: ".concat(e));
});
app.get('/api/envdata', function (req, res) { return __awaiter(void 0, void 0, void 0, function () {
    var data;
    var _a;
    return __generator(this, function (_b) {
        switch (_b.label) {
            case 0:
                _a = {};
                return [4 /*yield*/, getWeatherData()];
            case 1:
                data = (_a.weatherData = _b.sent(), _a);
                res.json(data);
                return [2 /*return*/];
        }
    });
}); });
app.post('/api/envdata', function (req, res) { return __awaiter(void 0, void 0, void 0, function () {
    var token, keyFound, data, isWeatherRecord, reason;
    return __generator(this, function (_a) {
        token = req.header("Authorization");
        if (token == undefined) {
            console.log("Unauthorized user tried to access API");
            res.status(401).send();
            return [2 /*return*/];
        }
        token = crypto_1.default.createHash("sha256").update(token).digest("hex");
        console.log("Got token of: ".concat(token));
        keyFound = false;
        {
            loadKeys(keysPath).forEach(function (val, indx) {
                if (val === token) {
                    keyFound = true;
                }
            });
        }
        if (!keyFound) {
            console.log("Unauthorized user tried to access API");
            res.status(401).send();
            return [2 /*return*/];
        }
        console.log("user authenticated");
        data = req.body;
        isWeatherRecord = isJsonWeatherRecord(data);
        reason = "OK";
        if (isWeatherRecord) {
            postWeatherData(data);
            res.status(200);
        }
        else {
            res.status(400);
            reason = "Invalid JSON data";
        }
        res.json({ reason: reason });
        return [2 /*return*/];
    });
}); });
app.listen(port, function () {
    console.log("Bradley Weather Station API listening on port ".concat(port));
});
