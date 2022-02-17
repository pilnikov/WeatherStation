
///-----------------------------------------------------------------------------WiFi.htm
			function loadVal_WiFi()
			{
				var xh = new XMLHttpRequest();
				xh.open("GET", "/jwifi", true);
				xh.send(null);
				xh.onreadystatechange =  function()
				{
					if (this.readyState == 4 && this.status == 200) 
					{
						var res = JSON.parse(xh.responseText);
						document.getElementById('aps' ).value = res.apid;
						document.getElementById('stas1').value = res.staid1;
						document.getElementById('stas2').value = res.staid2;
						document.getElementById('app' ).value = res.appas;
						document.getElementById('stap1').value = res.stapas1;
						document.getElementById('stap2').value = res.stapas2;
						document.getElementById('sst1').checked  = res.sst1;
						document.getElementById('sst2').checked  = res.sst2;

						document.getElementById('ipap').value = res.iap;
						document.getElementById('maap').value = res.map;
            
						var st = document.getElementById('sst1').checked;
						if (st)
						{
							document.getElementById('ipst1').value = res.ipst1;
							document.getElementById('mast1').value = res.mast1;
							document.getElementById('gwst1').value = res.gwst1;
							document.getElementById('dns1st1').value = res.dns1st1;
							document.getElementById('dns2st1').value = res.dns2st1;
						}
						st = document.getElementById('sst2').checked;
						if (st)
						{
							document.getElementById('ipst2').value = res.ipst2;
							document.getElementById('mast2').value = res.mast2;
							document.getElementById('gwst2').value = res.gwst2;
							document.getElementById('dns1st2').value = res.dns1st2;
							document.getElementById('dns2st2').value = res.dns2st2;
						}
						st_ip();
						document.getElementById('wof').checked  = res.wof;
					}
				};
				_from = 'WiFi';
			}
			
			function st_ip()
			{
				var st = document.getElementById('sst1').checked;
				
				showHide('ipst1',   st);
				showHide('mast1',   st);
				showHide('gwst1',   st);
				showHide('dns1st1', st);
				showHide('dns2st1', st);

				st = document.getElementById('sst2').checked;

				showHide('ipst2',   st);
				showHide('mast2',   st);
				showHide('gwst2',   st);
				showHide('dns1st2', st);
				showHide('dns2st2', st);
			}

			function set_wifi()
			{
				var wof_t = 0, st1= 0, st2 = 0;

				var as = document.getElementById('aps').value;
				var ap = document.getElementById('app').value;
				var ss1 = document.getElementById('stas1').value;
				var sp1 = document.getElementById('stap1').value;
				var ss2 = document.getElementById('stas2').value;
				var sp2 = document.getElementById('stap2').value;

				if (document.getElementById('sst1').checked) st1 = 1;
				if (document.getElementById('sst2').checked) st2 = 1;

				var iap = document.getElementById('ipap').value;
				var map = document.getElementById('maap').value;

				if (document.getElementById('wof').checked) wof_t = 1;

				url='/set_wifi?as='+as+'&ap='+ap+'&ss1='+ss1+'&sp1='+sp1+
				'&ss2='+ss2+'&sp2='+sp2+'&st1='+st1+'&st2='+st2+
				'&iap='+iap+'&map='+map+'&wof='+wof_t;

				var xh = new XMLHttpRequest();
				xh.open('GET', url, true);
				xh.send(null);
				xh.onreadystatechange = function()
				{
					if (this.readyState == 4 && this.status == 200) loadVal_WiFi();
				};      
				
				if (st1 === 1)
				{
					var ip1 = document.getElementById('ipst1').value;
					var ma1 = document.getElementById('mast1').value;
					var gw1 = document.getElementById('gwst1').value;
					var d11 = document.getElementById('dns1st1').value;
					var d21 = document.getElementById('dns2st1').value;

					url='/set_ip1?ip='+ip1+'&ma='+ma1+'&gw='+gw1+'&d1='+d11+'&d2='+d21;

					xh = new XMLHttpRequest();
					xh.open('GET', url, true);
					xh.send(null);
					xh.onreadystatechange = function()
					{
						if (this.readyState == 4 && this.status == 200) loadValues();
					}      
				}
				
				if (st2 === 1)
				{
					var ip2 = document.getElementById('ipst2').value;
					var ma2 = document.getElementById('mast2').value;
					var gw2 = document.getElementById('gwst2').value;
					var d12 = document.getElementById('dns1st2').value;
					var d22 = document.getElementById('dns2st2').value;

					url='/set_ip2?ip='+ip2+'&ma='+ma2+'&gw='+gw2+'&d1='+d12+'&d2='+d22;

					xh = new XMLHttpRequest();
					xh.open('GET', url, true);
					xh.send(null);
					xh.onreadystatechange = function()
					{
						if (this.readyState == 4 && this.status == 200) loadValues();
					};      
				}
			}
