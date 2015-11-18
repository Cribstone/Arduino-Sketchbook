# -*- coding: utf-8 -*-

###############################################################################
#
# GetEvent
# Retrieve data for all calendars in an account.
#
# Python version 2.6
#
###############################################################################

from temboo.core.choreography import Choreography
from temboo.core.choreography import InputSet
from temboo.core.choreography import ResultSet
from temboo.core.choreography import ChoreographyExecution

import json

class GetEvent(Choreography):

    def __init__(self, temboo_session):
        """
        Create a new instance of the GetEvent Choreo. A TembooSession object, containing a valid
        set of Temboo credentials, must be supplied.
        """
        Choreography.__init__(self, temboo_session, '/Library/Google/Calendar/GetEvent')


    def new_input_set(self):
        return GetEventInputSet()

    def _make_result_set(self, result, path):
        return GetEventResultSet(result, path)

    def _make_execution(self, session, exec_id, path):
        return GetEventChoreographyExecution(session, exec_id, path)

class GetEventInputSet(InputSet):
    """
    An InputSet with methods appropriate for specifying the inputs to the GetEvent
    Choreo. The InputSet object is used to specify input parameters when executing this Choreo.
    """
    def set_AccessToken(self, value):
        """
        Set the value of the AccessToken input for this Choreo. ((optional, string) A valid access token retrieved during the OAuth process. This is required unless you provide the ClientID, ClientSecret, and RefreshToken to generate a new access token.)
        """
        InputSet._set_input(self, 'AccessToken', value)
    def set_CalendarID(self, value):
        """
        Set the value of the CalendarID input for this Choreo. ((required, string) The unique ID for the calendar with the events to retrieve.)
        """
        InputSet._set_input(self, 'CalendarID', value)
    def set_ClientID(self, value):
        """
        Set the value of the ClientID input for this Choreo. ((conditional, string) The Client ID provided by Google. Required unless providing a valid AccessToken.)
        """
        InputSet._set_input(self, 'ClientID', value)
    def set_ClientSecret(self, value):
        """
        Set the value of the ClientSecret input for this Choreo. ((conditional, string) The Client Secret provided by Google. Required unless providing a valid AccessToken.)
        """
        InputSet._set_input(self, 'ClientSecret', value)
    def set_EventID(self, value):
        """
        Set the value of the EventID input for this Choreo. ((required, string) The unique ID for the event to retrieve.)
        """
        InputSet._set_input(self, 'EventID', value)
    def set_RefreshToken(self, value):
        """
        Set the value of the RefreshToken input for this Choreo. ((conditional, string) An OAuth Refresh Token used to generate a new access token when the original token is expired. Required unless providing a valid AccessToken.)
        """
        InputSet._set_input(self, 'RefreshToken', value)
    def set_ResponseFormat(self, value):
        """
        Set the value of the ResponseFormat input for this Choreo. ((optional, string) The format that response should be in. Can be set to xml or json. Defaults to json.)
        """
        InputSet._set_input(self, 'ResponseFormat', value)

class GetEventResultSet(ResultSet):
    """
    A ResultSet with methods tailored to the values returned by the GetEvent Choreo.
    The ResultSet object is used to retrieve the results of a Choreo execution.
    """
    		
    def getJSONFromString(self, str):
        return json.loads(str)
    
    def get_Response(self):
        """
        Retrieve the value for the "Response" output from this Choreo execution. (The response from Google. Corresponds to the ResponseFormat input. Defaults to JSON.)
        """
        return self._output.get('Response', None)
    def get_NewAccessToken(self):
        """
        Retrieve the value for the "NewAccessToken" output from this Choreo execution. ((string) Contains a new AccessToken when the RefreshToken is provided.)
        """
        return self._output.get('NewAccessToken', None)

class GetEventChoreographyExecution(ChoreographyExecution):
    
    def _make_result_set(self, response, path):
        return GetEventResultSet(response, path)
