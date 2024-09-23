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

CONF_SENSOR1 = "speed"
CONF_SENSOR2 = "cadence"
CONF_SENSOR3 = "power"
CONF_SENSOR4 = "distance"
CONF_SENSOR5 = "heart"
CONF_SENSOR6 = "workout_time"
CONF_SENSOR7 = "calories"
CONF_AGE = "user_age"
CONF_WEIGHT = "user_weight"
CONF_SEX = "user_sex"

# This schema is for the 'exercise_sensor:' block in YAML
EXERCISE_SENSOR_SCHEMA = cv.Schema({
    cv.GenerateID(): cv.declare_id(ExerciseSensor),
    cv.Optional(CONF_AGE, default=30): cv.positive_int,
})

CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(): cv.declare_id(ExerciseSensor),
        cv.Optional(CONF_SENSOR1): sensor.sensor_schema(unit_of_measurement=UNIT_EMPTY, icon=ICON_EMPTY, accuracy_decimals=2),
        cv.Optional(CONF_SENSOR2): sensor.sensor_schema(unit_of_measurement=UNIT_EMPTY, icon=ICON_EMPTY, accuracy_decimals=2),
        cv.Optional(CONF_SENSOR3): sensor.sensor_schema(unit_of_measurement=UNIT_WATT, icon=ICON_EMPTY, accuracy_decimals=2),
        cv.Optional(CONF_SENSOR4): sensor.sensor_schema(unit_of_measurement=UNIT_EMPTY, icon=ICON_EMPTY, accuracy_decimals=3),
        cv.Optional(CONF_SENSOR5): sensor.sensor_schema(unit_of_measurement=UNIT_EMPTY, icon=ICON_EMPTY, accuracy_decimals=0),
        cv.Optional(CONF_SENSOR6): sensor.sensor_schema(unit_of_measurement=UNIT_EMPTY, icon=ICON_EMPTY, accuracy_decimals=2),
        cv.Optional(CONF_SENSOR7): sensor.sensor_schema(unit_of_measurement=UNIT_EMPTY, icon=ICON_EMPTY, accuracy_decimals=0),
        cv.Optional(CONF_AGE, default=30): cv.positive_int,  # New configuration for age with a default
        cv.Optional(CONF_WEIGHT, default=70): cv.positive_int,  # New configuration for weight with a default
        cv.Optional(CONF_SEX, default=0): cv.positive_int,  # New configuration for sex with a default
    }
).extend(cv.polling_component_schema("1s"))


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)

    if sensor_1_config := config.get(CONF_SENSOR1):
        sens = await sensor.new_sensor(sensor_1_config)
        cg.add(var.set_sensor1(sens))

    if sensor_2_config := config.get(CONF_SENSOR2):
        sens = await sensor.new_sensor(sensor_2_config)
        cg.add(var.set_sensor2(sens))

    if sensor_3_config := config.get(CONF_SENSOR3):
        sens = await sensor.new_sensor(sensor_3_config)
        cg.add(var.set_sensor3(sens))

    if sensor_4_config := config.get(CONF_SENSOR4):
        sens = await sensor.new_sensor(sensor_4_config)
        cg.add(var.set_sensor4(sens))

    if sensor_5_config := config.get(CONF_SENSOR5):
        sens = await sensor.new_sensor(sensor_5_config)
        cg.add(var.set_sensor5(sens))

    if sensor_6_config := config.get(CONF_SENSOR6):
        sens = await sensor.new_sensor(sensor_6_config)
        cg.add(var.set_sensor6(sens))

    if sensor_7_config := config.get(CONF_SENSOR7):
        sens = await sensor.new_sensor(sensor_7_config)
        cg.add(var.set_sensor7(sens))

    cg.add(var.set_age(config[CONF_AGE]))
    cg.add(var.set_weight(config[CONF_WEIGHT]))
    cg.add(var.set_sex(config[CONF_SEX]))