# QEJson

It is a JSON serialization library of **QE Framework**. It allows you to serialize *QObjects* into JSON format using the object's properties and annotations.

[![badge](https://img.shields.io/badge/conan.io-QEJson%2F1.0.0-green.svg?logo=data:image/png;base64%2CiVBORw0KGgoAAAANSUhEUgAAAA4AAAAOCAMAAAAolt3jAAAA1VBMVEUAAABhlctjlstkl8tlmMtlmMxlmcxmmcxnmsxpnMxpnM1qnc1sn85voM91oM11oc1xotB2oc56pNF6pNJ2ptJ8ptJ8ptN9ptN8p9N5qNJ9p9N9p9R8qtOBqdSAqtOAqtR%2BrNSCrNJ/rdWDrNWCsNWCsNaJs9eLs9iRvNuVvdyVv9yXwd2Zwt6axN6dxt%2Bfx%2BChyeGiyuGjyuCjyuGly%2BGlzOKmzOGozuKoz%2BKqz%2BOq0OOv1OWw1OWw1eWx1eWy1uay1%2Baz1%2Baz1%2Bez2Oe02Oe12ee22ujUGwH3AAAAAXRSTlMAQObYZgAAAAFiS0dEAIgFHUgAAAAJcEhZcwAACxMAAAsTAQCanBgAAAAHdElNRQfgBQkREyOxFIh/AAAAiklEQVQI12NgAAMbOwY4sLZ2NtQ1coVKWNvoc/Eq8XDr2wB5Ig62ekza9vaOqpK2TpoMzOxaFtwqZua2Bm4makIM7OzMAjoaCqYuxooSUqJALjs7o4yVpbowvzSUy87KqSwmxQfnsrPISyFzWeWAXCkpMaBVIC4bmCsOdgiUKwh3JojLgAQ4ZCE0AMm2D29tZwe6AAAAAElFTkSuQmCC)](http://www.conan.io/source/QEJson/1.0.0/fmiguelgarcia/stable)

## How to use it

It uses the same annotations that are available for [QEOrm](https://github.com/fmiguelgarcia/QEOrm). Following the example in QEOrm, to get an object of class 'Book' serilized to JSON:

```C++
	Book b1;
	// ... fill b1 properties.

	qe::json::SerializedItem target;
	QEJson::instance().save( &book, &target);

	// Get QJson result...
	QJsonObject jsonB1 = target.value().toObject();


	// Loading an object from QJsonObject.
	Book b2;
	QEJson::instance().load( &target, &b2);

```

You can see more examples into 'test' folder.

## Installation

The easiest way to use this library in your code is by [Conan Package Manager](https://www.conan.io).
Please use the project setup that you can find in [QEJson/1.0.0 Conan](https://www.conan.io/source/QEJson/1.0.0/fmiguelgarcia/stable).

If you use *CMake*, steps after adding this dependency to your "conanfile.py" are as usual:
```bash
	$~/yourProject > mkdir build
	$~/yourProject > cd build
	$~/yourProject/build > conan install -s build_type=debug .. 
	$~/yourProject/build > cmake -DCMAKE_BUILD_TYPE=Debug  .. 
	$~/yourProject/build > cmake --build .
```

## Versioning

This library uses [Semantic Versioning](htpp://semver.org) and internal private implementation to provide a long-term, stable, and reusable binaries.

## License 

This license follows the dual-license pattern that Qt is currently using. A [LGPL 3.0 license](https://www.gnu.org/licenses/lgpl-3.0-standalone.html) for projects that do not need commercial support, and another one, **Commercial license** for rest of cases. 

You can find a brief comparison between both licenses in the below table:

 License types                 | LGPLv3 | Commercial 
-------------------------------|--------|------------
 Community Support             | Yes    | Yes
 Official Support Helpdesk     | No     | Yes
 Keep your application private | No     | Yes
 Possible to keep your application private with dynamic linking | Yes | Yes
 No need to provide a relinking mechanism for QE Framework <br> (can always use static linking)  | No | Yes
 No need to provide a copy of license and explicitly<br> acknowledge the use of QE | No | Yes
 No need to make copy of the QE Framework source code <br> available for customers | No | Yes
 Full rights to proprietary QE source code modifications | No | Yes
 Enforce DRM                   | [See LGPLv3 FAQ](https://www.gnu.org/licenses/gpl-faq.html#DRMProhibited) | Yes
 Enforce software patents      | [See LGPLv3 FAQ](https://www.gnu.org/licenses/gpl-faq.html#DRMProhibited) | Yes

