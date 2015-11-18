# -*- coding: utf-8 -*-

###############################################################################
#
# UpdateCalendar
# Updates the metadata for a calendar.
#
# Python version 2.6
#
###############################################################################

from temboo.core.choreography import Choreography
from temboo.core.choreography import InputSet
from temboo.core.choreography import ResultSet
from temboo.core.choreography import ChoreographyExecution

import json

class UpdateCalendar(Choreography):

    def __init__(self, temboo_session):
        """
        Create a new instance of the UpdateCalendar Choreo. A TembooSession object, containing a valid
        set of Temboo credentials, must be supplied.
        """
        Choreography.__init__(self, temboo_session, '/Library/Google/Calendar/UpdateCalendar')


    def new_input_set(self):
        return UpdateCalendarInputSet()

    def _make_result_set(self, result, path):
        return UpdateCalendarResultSet(result, path)

    def _make_execution(self, session, exec_id, path):
        return UpdateCalendarChoreographyExecution(session, exec_id, path)

class UpdateCalendarInputSet(InputSet):
    """
    An InputSet with methods appropriate for specifying the inputs to the UpdateCalendar
    Choreo. The InputSet object is used to specify input parameters when executing this Choreo.
    """
    def set_AccessToken(self, value):
        """
        Set the value of the AccessToken input for this Choreo. ((optional, string) A valid access token retrieved during the OAuth process. This is required unless you provide the ClientID, ClientSecret, and RefreshToken to generate a new access token.)
        """
        InputSet._set_input(self, 'AccessToken', value)
    def set_CalendarID(self, value):
        """
        Set the value of the CalendarID input for this Choreo. ((required, string) The ID for the calendar to update.)
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
    def set_NewDescription(self, value):
        """
        Set the value of the NewDescription input for this Choreo. ((optional, string) The new description for the calendar to update.)
        """
        InputSet._set_input(self, 'NewDescription', value)
    def set_NewLocation(self, value):
        """
        Set the value of the NewLocation input for this Choreo. ((optional, string) The new location for the calendar to update.)
        """
        InputSet._set_input(self, 'NewLocation', value)
    def set_NewSummary(self, value):
        """
        Set the value of the NewSummary input for this Choreo. ((required, string) The new summary for the calendar to update.)
        """
        InputSet._set_input(self, 'NewSummary', value)
    def set_NewTimezone(self, value):
        """
        Set the value of the NewTimezone input for this Choreo. ((optional, string) The new timezone for the calendar to update.)
        """
        InputSet._set_input(self, 'NewTimezone', value)
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

class UpdateCalendarResultSet(ResultSet):
    """
    A ResultSet with methods tailored to the values returned by the UpdateCalendar Choreo.
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

class UpdateCalendarChoreographyExecution(ChoreographyExecution):
    
    def _make_result_set(self, response, path):
        return UpdateCalendarResultSet(response, path)
