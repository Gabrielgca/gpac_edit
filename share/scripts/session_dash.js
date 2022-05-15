 import { Sys as sys } from 'gpaccore';
 //import * as editInfo from 'C:\\Repositorios\\gpac_public\\share\\scripts\\editInfo.json';
 import editInfoJSON from 'C:\\Repositorios\\gpac_public\\share\\scripts\\edit_files\\editInfo.js';

 _gpac_log_name="ABR-EDIT";

print("Hello GPAC from SCRIPTS!");

let all_filters = [];

//custom rate adaptation object
let dashin = {};
dashin.groups = [];

let compositor = null;

print(JSON.stringify(editInfoJSON));

session.set_new_filter_fun( (f) => {
	print("new filter " + f.name);
	f.iname = "JS"+f.name;
	all_filters.push(f);

	//bind our custom rate adaptation !
	if (f.name == "dashin") {
		f.bind(dashin);
	}
	else if (f.name == "compositor"){
		compositor = f;
		print("COMPOSITOR SENDO GUARDADO!");
	}

} ); 

session.set_del_filter_fun( (f) => {
print("delete filter " + f.iname);
let idx = all_filters.indexOf(f);
if (idx>=0)
	all_filters.splice (idx, 1);
}); 


// Event constants linked to the user events. The list can be found in the event_constants.h file
const GF_EVENT_EDIT = 127;
const GF_EVENT_DRAWN_FRAME = 128;

// Filter Event User constant. The list can be found in the filter.h file
const GF_FEVT_USER = 17;

let nb_adapt = 0;
let frame_nb = 0;
let last_frame_with_edit = 0;
let next_edit = 0;

session.set_event_fun( (evt) => {
	if (evt.type != GF_FEVT_USER) return 0;

	switch (evt.ui_type) {
		case GF_EVENT_DRAWN_FRAME:
			print("Drawn frame on JS");
			print("frame number: " + evt.drawn_frame);
			frame_nb = evt.drawn_frame;
			handleDrawnFrame(frame_nb);
			break;
	}
	});

const handleDrawnFrame = (frame_nb) => {
	if (editInfoJSON[next_edit] && editInfoJSON[next_edit]["frame"] == frame_nb && last_frame_with_edit != frame_nb)
		fireRotation(-500);
}

/* Fire an edit event to do a rotation on the video camera. The rotation happens in the next frame.
 * @param rotation -> how many pixels to rotate. negative values do rotation to the left and positive to the right.
 */
const fireRotation = (rotation) => {
	let f_evt = new FilterEvent(GF_FEVT_USER);

	f_evt.ui_type = GF_EVENT_EDIT;
	f_evt.edit_rotation = rotation;

	let result = session.fire_event(f_evt, compositor, false);

	// Update the last frame that happened an edit so it does happen again
	last_frame_with_edit = frame_nb;
	next_edit++;

	return result;
}


let groups = [];

function get_group(group_idx) 
{
	for (let i=0; i < groups.length; i++) {
		if (groups[i].idx == group_idx)
			return groups[i];
	}

	return null;
}

const get_minimum_quality = () => {
	for (var i = 0; i < QualitiesInformation.length; i++)
	{
		if (QualitiesInformation[i].quality == 0)
			return QualitiesInformation[i];
	}

	return null;
}

const bola_find_max_utility_index = (utility_rep ,V, gamma, p, Q) => {
	var max_utility = -1;
	var new_index = -1;

	for (var k = 0; k < QualitiesInformation.length; k++) {
		var utility = (V * utility_rep[k] + V*gamma*p - Q) / (QualitiesInformation[k].bitrate*p);
		if (utility >= max_utility) {
			max_utility = utility;
			new_index = k;
		}
	}
	print ("New index from max utility index: " + new_index);
	return new_index;
}

const get_max_quality_below = (rate) => {
	var max_rate = 0;
	for (var k = QualitiesInformation.length -1; k >=0; k--) {
		if (QualitiesInformation[k].bitrate > max_rate && QualitiesInformation[k].bitrate < rate)
		{
			return QualitiesInformation[k].quality;
		}
	}
	// lowest waulity 
	return QualitiesInformation[0].quality;
}


//############## LINEAR REGRESSION ##############//

const funcao_linear = (a, b, x) => {
    return a*x + b;
}

function linearRegression(y,x){
	var lr = {};
	var n = y.length;
	var sum_x = 0;
	var sum_y = 0;
	var sum_xy = 0;
	var sum_xx = 0;
	var sum_yy = 0;

	for (var i = 0; i < y.length; i++) {

		sum_x += x[i];
		sum_y += y[i];
		sum_xy += (x[i]*y[i]);
		sum_xx += (x[i]*x[i]);
		sum_yy += (y[i]*y[i]);
	} 

	lr['slope'] = (n * sum_xy - sum_x * sum_y) / (n*sum_xx - sum_x * sum_x);
	lr['intercept'] = (sum_y - lr.slope * sum_x)/n;
	lr['r2'] = Math.pow((n*sum_xy - sum_x*sum_y)/Math.sqrt((n*sum_xx-sum_x*sum_x)*(n*sum_yy-sum_y*sum_y)),2);

	return lr;
}

//##########################################//


var currentBuffer = 0;
var currentWidth = 0;
var currentHeight = 0;
var retrieveQualitiesInformation = true;
const QualitiesInformation = [];
var utility = [];

//BOLA VARIANTS
const BOLA_BASIC = 0;
const BOLA_FINITE = 1;
const BOLA_U = 2;
const BOLA_O = 3;

const BOLA_VARIANT = BOLA_BASIC;

const nb_samples_viewport = 10;
const center_viewport_x = [];
const center_viewport_y = [];
const nb_adapt_array = [];


var new_width = undefined;
var new_height = undefined;

dashin.rate_adaptation = function (group_idx, base_group_idx, force_lower_complexity, stats)
{
	print("###############################################")
	print(`Getting called in custom algo ! group ${group_idx} base_group ${base_group_idx} force_lower_complexity ${force_lower_complexity}`);
	print('Stats: ' + JSON.stringify(stats));
	
	let group = get_group(group_idx);
	print('Group: ' + JSON.stringify(group));

	let base_group = get_group(base_group_idx);

	if (!group) return -1;

	//Base group have the whole video buffer information
	if (group_idx == 0)
	{	
		currentBuffer = stats.buffer;
		currentWidth = stats.display_width;
		currentHeight = stats.display_height;
		nb_adapt++;

		print('\n\nnb_adapt :' + nb_adapt);

		center_viewport_x.push(stats.center_viewport_x);
		center_viewport_y.push(stats.center_viewport_y);
		nb_adapt_array.push(nb_adapt);

		if (nb_adapt_array.length > nb_samples_viewport)
		{
			center_viewport_x.shift();
			center_viewport_y.shift();
			nb_adapt_array.shift();
		}

		var lr_width = linearRegression(center_viewport_x, nb_adapt_array);

		var lr_height = linearRegression(center_viewport_y, nb_adapt_array);

		print("A_WIDTH: " + lr_width.slope + ",B_WIDTH: " + lr_width.intercept);
		print("A_HEIGHT: " + lr_height.slope + ",B_HEIGHT: " + lr_height.intercept);

		new_width = funcao_linear(lr_width.slope, lr_width.intercept, nb_adapt_array[nb_adapt_array.length -1] + 1);
		new_height = funcao_linear(lr_height.slope, lr_height.intercept, nb_adapt_array[nb_adapt_array.length -1] + 1);

		
	}

	var new_index = -1;
	//TODO: VERIFICAR A DURAÇÃO DO CHUNK
	//group->current_downloaded_segment_duration/1000.0,
	const p = group.duration / 1000.0;	// segment duration
	const gamma = 5/p;
	const Qmax = stats.buffer_max / 1000.0 / p;		// max nb of segments in the buffer
	const Q = currentBuffer / 1000.0 / p; // current nb of segments in buffer

	if (BOLA_VARIANT == BOLA_BASIC)
	{	
		const V = (Qmax - 1) / (gamma * p + /*Max utility*/ utility[utility.length -1]);

		new_index =  bola_find_max_utility_index(utility, V, gamma, p, Q);
	}
	else if (BOLA_VARIANT == BOLA_FINITE || BOLA_VARIANT == BOLA_U || BOLA_VARIANT == BOLA_O)
	{
		/* BOLA FINITE is the same as BOLA Basic with the wind-up and down phases */
		/* BOLA O and U add extra steps to BOLA FINITE */
		const N = base_group.duration / p;
		const t_bgn = p*nb_adapt; //play time from begin
		const t_end = (N - nb_adapt)*p; //play time to the end
		const t = Math.min(t_bgn, t_end);
		const t_prime = Math.max(t / 2, 3 * p);
		const Q_Dmax  = Math.min(Qmax, t_prime / p);

		const V_D = (Q_Dmax - 1) / (gamma * p + /*Max utility*/ utility[utility.length -1]);

		new_index =  bola_find_max_utility_index(utility ,V_D, gamma, p, Q);

		if (BOLA_VARIANT == BOLA_U || BOLA_VARIANT == BOLA_O)
		{
			//Bola U algorithm
			if ((new_index != -1) && (new_index > stats.active_quality))
			{
				//last segment download rate in bits per second
				const r = stats.rate;

				var m_prime = 0;
				m_prime = get_max_quality_below(Math.max(r, QualitiesInformation[0].bitrate));
				if (m_prime >= new_index)
				{
					m_prime = new_index;
				}
				else if (m_prime < stats.active_quality)
				{
					m_prime = stats.active_quality;
				}
				else
				{
					if (BOLA_VARIANT == BOLA_U)
					{
						m_prime++;
					}
					else (BOLA_VARIANT == BOLA_O)
					{
						//TODO: GF_DASH_ALGO_BOLA_O
						//Requires pause event
					}
				}
				new_index = m_prime;
			}
		}
		// TODO trigger pause for max(p*(Q-Q_Dmax+1), 0)
	}
	print("[DASH - CUSTOM] BOLA: buffer " + currentBuffer + " ms, segment number " + nb_adapt + ", new quality "+ new_index +" with rate " + QualitiesInformation[new_index].bitrate)
	print("[DASH - CUSTOM] VIEWPORT: Current height: " + currentHeight + ", Current width: " + currentWidth);
	print("[DASH - CUSTOM] CENTER VIEWPORT WIDTH: [" + center_viewport_x + "]");
	print("[DASH - CUSTOM] CENTER VIEWPORT HEIGHT: [" + center_viewport_y + "]");

	print("New Width => " + new_width);
	print("New Height => " + new_height);
	return new_index;
}


dashin.new_group = function (group)
{
	//remember the group (adaptation set in dash)
	print("New group: " + JSON.stringify(group));

	if (retrieveQualitiesInformation && group.idx != 0){
		for (var i = 0; i < group.qualities.length; i++){
			QualitiesInformation.push({quality: i, bitrate: group.qualities[i].bitrate})
		}
		// Computing the log-based utility of each segment (recomputing each time for period changes)
		for (var k = 0; k < QualitiesInformation.length; k++)
		{
		utility[k] = Math.log(QualitiesInformation[k].bitrate / get_minimum_quality().bitrate);
		}
		retrieveQualitiesInformation = false;
	}
	print("New Qualities Information: ");
	
	QualitiesInformation.forEach(function(entry) {
		print(JSON.stringify(entry));
	});
	print("Utility Array: ");
	utility.forEach(function(entry) {
		print(entry);
	});
	groups.push(group);
}

dashin.period_reset = function (reset_type)
{
	print("Period reset type " + reset_type);
	if (!reset_type)
		groups = [];
}

dashin.download_monitor = function (group_idx, stats)
{
	print("Download info " + JSON.stringify(stats));
	return -1;
}