import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import sensor
from esphome.const import CONF_ID, UNIT_EMPTY, ICON_EMPTY, UNIT_WATT

exercise_sensor_ns = cg.esphome_ns.namespace("exercise_sensor")
ExerciseSensor = exercise_sensor_ns.class_("ExerciseSensor", cg.PollingComponent)

CONF_SENSOR1 = "speed"
CONF_SENSOR2 = "cadence"
CONF_SENSOR3 = "power"
CONF_SENSOR4 = "distance"

CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(): cv.declare_id(ExerciseSensor),
        cv.Optional(CONF_SENSOR1): sensor.sensor_schema(unit_of_measurement=UNIT_EMPTY, icon=ICON_EMPTY, accuracy_decimals=2),
        cv.Optional(CONF_SENSOR2): sensor.sensor_schema(unit_of_measurement=UNIT_EMPTY, icon=ICON_EMPTY, accuracy_decimals=2),
        cv.Optional(CONF_SENSOR3): sensor.sensor_schema(unit_of_measurement=UNIT_WATT, icon=ICON_EMPTY, accuracy_decimals=2),
        cv.Optional(CONF_SENSOR4): sensor.sensor_schema(unit_of_measurement=UNIT_EMPTY, icon=ICON_EMPTY, accuracy_decimals=3),
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
