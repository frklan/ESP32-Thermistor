# ESP32-Thermistor

This is a very simple ESP32-idf driver for a Thermistor based temperature sensor.

To be used as a component in a ESP-IDF project.

## Circuit

Wire the thermistor to one of the adc1 inputs on an ESP32 board via a 10k resistor

```
        o vcc
        │
       ┌┴┐
       │ │ 10k
       └┬┘
ADC     │
o───────┤
        │
       ┌┴┐╱
       │╱│ Thermistor
      ╱└┬┘
        │
       ═╧═
```

## Contributing

Contributions are always welcome!

When contributing to this repository, please first discuss the change you wish to make via the issue tracker, email, or any other method with the owner of this repository before making a change.

Please note that we have a code of conduct, you are required to follow it in all your interactions with the project.

## Versioning

We use [SemVer](http://semver.org/) for versioning. For the versions available, see the [tags on this repository](https://github.com/frklan/GameEngine/tags).

## Authors

* **Fredrik Andersson** - *Initial work* - [frklan](https://github.com/frklan)

## License

This project is licensed under the CC BY NA License - see the [LICENSE](LICENSE) file for details

