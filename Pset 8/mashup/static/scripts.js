// Google Map
var map;

// markers for map
var markers = [];

// info window
var info = new google.maps.InfoWindow();

// execute when the DOM is fully loaded
$(function() {

    // styles for map
    // https://developers.google.com/maps/documentation/javascript/styling
    var styles = [
        
        // turn off land parcel labels
        {
            "featureType": "administrative.land_parcel",
            "stylers": [
                {"visibility": "off"}
            ]
        },
        
        // turn off neighborhood labels
        {
            "featureType": "administrative.neighborhood",
            "stylers": [
                {"visibility": "off"}
            ]
        },
        
        // turn off point of interest labels
        {
            "featureType": "poi",
            "elementType": "labels.text",
            "stylers": [
                {"visibility": "off"}
            ]
        },
        
        // turn off business points of interest
        {
            "featureType": "poi.business",
            "stylers": [
                {"visibility": "off"}
            ]
        },
        
        // turn off road labels
        {
            "featureType": "road",
            "elementType": "labels",
            "stylers": [
                {"visibility": "off"}
            ]
        },
        
        // turn off road icons
        {
            "featureType": "road",
            "elementType": "labels.icon",
            "stylers": [
                {"visibility": "off"}
            ]
        },
        
        // turn off transit
        {
            "featureType": "transit",
            "stylers": [
                {"visibility": "off"}
            ]
        },
        
        // turn off water body labels
        {
            "featureType": "water",
            "elementType": "labels.text",
            "stylers": [
                {"visibility": "off"}
            ]
        }
    ];

    // options for map
    // https://developers.google.com/maps/documentation/javascript/reference#MapOptions
    var options = {
        center: {lat: 40.772522, lng: -73.974338}, // Central Park, New York City, New York
        disableDefaultUI: true,
        mapTypeId: google.maps.MapTypeId.ROADMAP,
        maxZoom: 14,
        panControl: true,
        styles: styles,
        zoom: 13,
        zoomControl: true
    };

    // get DOM node in which map will be instantiated
    var canvas = $("#map-canvas").get(0);

    // instantiate map
    map = new google.maps.Map(canvas, options);

    // configure UI once Google Map is idle (i.e., loaded)
    google.maps.event.addListenerOnce(map, "idle", configure);

});

/**
 * Adds marker for place to map.
 */
function addMarker(place)
{
    // add a marker for a place on the map, where place is a JavaScript object that represents a row from places
    
    // get the latitude and longitude for marker placement
    var myLatLng = new google.maps.LatLng(place["latitude"], place["longitude"]);
    
    // get the icon for the marker
    var icon_image = {
        url: "http://maps.google.com/mapfiles/kml/pal2/icon23.png",
        labelOrigin: new google.maps.Point(20, 45) // position the label below the marker
    };
    
    // instantiate the marker
    var marker = new google.maps.Marker({
        position: myLatLng,
        map: map,
        title: place["place_name"] + ", " + place["admin_name1"], // City, State
        label: place["place_name"] + ", " + place["admin_name1"], // City, State
        icon: icon_image
    });
    
    // get relevant articles for the place marker
    // $.getJSON(url, data, success)
    // url: a string containing the URL to which the request is sent
    // data: a plain object or string that is sent to the server with the request
    // success: a callback function that is executed if the request succeeds
    $.getJSON(Flask.url_for("articles"), {geo: place.postal_code}, function(articles) {
        
        // only display the information window if there are articles
        if (!$.isEmptyObject(articles)) {
            
            // start an unordered list
            var articlesList = "<ul>";
            
            for (var i = 0; i < articles.length; i++) {
                // create each article list item
                // a href = link for each article (articles[i].link)
                // a innerHTML = title for each article (articles[i].title)
                articlesList += "<li><a target='_blank' href='" + articles[i].link
                + "'>" + articles[i].title + "</a></li>";
            }
        }
            
        // close the unordered list element
        articlesList += "</ul>";
            
        // listen for click event on the marker
        google.maps.event.addListener(marker, 'click', function () {
            // show the information window with a list of articles
            showInfo(marker, articlesList);
        });
    });
    
    // add the marker we just created to the array of markers
    markers.push(marker);
}

/**
 * Configures application.
 */
function configure()
{
    // update UI after map has been dragged
    google.maps.event.addListener(map, "dragend", function() {

        // if info window isn't open
        // http://stackoverflow.com/a/12410385
        if (!info.getMap || !info.getMap())
        {
            update();
        }
    });

    // update UI after zoom level changes
    google.maps.event.addListener(map, "zoom_changed", function() {
        update();
    });

    // configure typeahead
    $("#q").typeahead({
        highlight: false,
        minLength: 1
    },
    {
        display: function(suggestion) { return null; },
        limit: 10,
        source: search,
        templates: {
            suggestion: Handlebars.compile(
                "<div>" +
                "{{ place_name }}, {{ admin_name1 }}, {{ postal_code }}" +
                "</div>"
            )
        }
    });

    // re-center map after place is selected from drop-down
    $("#q").on("typeahead:selected", function(eventObject, suggestion, name) {

        // set map's center
        map.setCenter({lat: parseFloat(suggestion.latitude), lng: parseFloat(suggestion.longitude)});

        // update UI
        update();
    });

    // hide info window when text box has focus
    $("#q").focus(function(eventData) {
        info.close();
    });

    // re-enable ctrl- and right-clicking (and thus Inspect Element) on Google Map
    // https://chrome.google.com/webstore/detail/allow-right-click/hompjdfbfmmmgflfjdlnkohcplmboaeo?hl=en
    document.addEventListener("contextmenu", function(event) {
        event.returnValue = true; 
        event.stopPropagation && event.stopPropagation(); 
        event.cancelBubble && event.cancelBubble();
    }, true);

    // update UI
    update();

    // give focus to text box
    $("#q").focus();
}

/**
 * Removes markers from map.
 */
function removeMarkers()
{
    // remove all the markers from the map
    for (var i = 0; i < markers.length; i++) {
        markers[i].setMap(null);
    }
    
    // delete the markers
    markers = [];
}

/**
 * Searches database for typeahead's suggestions.
 */
function search(query, syncResults, asyncResults)
{
    // get places matching query (asynchronously)
    var parameters = {
        q: query
    };
    $.getJSON(Flask.url_for("search"), parameters)
    .done(function(data, textStatus, jqXHR) {
     
        // call typeahead's callback with search results (i.e., places)
        asyncResults(data);
    })
    .fail(function(jqXHR, textStatus, errorThrown) {

        // log error to browser's console
        console.log(errorThrown.toString());

        // call typeahead's callback with no results
        asyncResults([]);
    });
}

/**
 * Shows info window at marker with content.
 */
function showInfo(marker, content)
{
    // start div
    var div = "<div id='info'>";
    if (typeof(content) == "undefined")
    {
        // http://www.ajaxload.info/
        div += "<img alt='loading' src='/static/ajax-loader.gif'/>";
    }
    else
    {
        div += content;
    }

    // end div
    div += "</div>";

    // set info window's content
    info.setContent(div);

    // open info window (if not already open)
    info.open(map, marker);
}

/**
 * Updates UI's markers.
 */
function update() 
{
    // get map's bounds
    var bounds = map.getBounds();
    var ne = bounds.getNorthEast();
    var sw = bounds.getSouthWest();

    // get places within bounds (asynchronously)
    var parameters = {
        ne: ne.lat() + "," + ne.lng(),
        q: $("#q").val(),
        sw: sw.lat() + "," + sw.lng()
    };
    $.getJSON(Flask.url_for("update"), parameters)
    .done(function(data, textStatus, jqXHR) {

       // remove old markers from map
       removeMarkers();

       // add new markers to map
       for (var i = 0; i < data.length; i++)
       {
           addMarker(data[i]);
       }
    })
    .fail(function(jqXHR, textStatus, errorThrown) {

        // log error to browser's console
        console.log(errorThrown.toString());
    });
};
