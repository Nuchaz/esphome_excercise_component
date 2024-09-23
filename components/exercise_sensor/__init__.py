import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import sensor
from esphome.const import CONF_ID, UNIT_EMPTY, ICON_EMPTY, UNIT_WATT

CODEOWNERS = ["@Nuchaz"]
DEPENDENCIES = ["esp32"]  # Add any dependencies your component has
AUTO_LOAD = ["sensor"]

CONF_EXERCISE_SENSOR_ID = "exercise_sensor_id"
exercise_sensor_ns = cg.esphome_ns.namespace("exercise_sensor")
ExerciseSensor = exercise_sensor_ns.class_("ExerciseSensor", cg.PollingComponent)

CONF_AGE = "user_age"
CONF_WEIGHT = "user_weight"
CONF_SEX = "user_sex"

# This schema is for the 'exercise_sensor:' block in YAML
EXERCISE_SENSOR_SCHEMA = cv.Schema({
    cv.GenerateID(): cv.declare_id(ExerciseSensor),
    cv.Optional(CONF_AGE, default=30): cv.positive_int,
    cv.Optional(CONF_WEIGHT, default=70): cv.positive_int,
    cv.Optional(CONF_SEX, default=0): cv.positive_int,
})

CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(): cv.declare_id(ExerciseSensor),
        cv.Optional(CONF_AGE, default=30): cv.positive_int,  # New configuration for age with a default
        cv.Optional(CONF_WEIGHT, default=70): cv.positive_int,  # New configuration for age with a default
        cv.Optional(CONF_SEX, default=0): cv.positive_int,  # New configuration for age with a default
    }
).extend(cv.polling_component_schema("1s"))


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    cg.add(var.set_age(config[CONF_AGE]))
    cg.add(var.set_weight(config[CONF_WEIGHT]))
    cg.add(var.set_sex(config[CONF_SEX]))